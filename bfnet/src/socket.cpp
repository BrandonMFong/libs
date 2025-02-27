/**
 * author: brando
 * date: 2/1/24
 */

#include "socket.hpp"
#include "server.hpp"
#include "connection.hpp"
#include "client.hpp"
#include "envelope.hpp"
#include <netinet/in.h> //structure for storing address information 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/socket.h> //for socket APIs 
#include <sys/types.h> 
#include <unistd.h>
#include <bflibcpp/bflibcpp.hpp>
#include <bflibc/bflibc.h>
#include "internal/log.hpp"

using namespace BF;
using namespace BF::Net;

Socket * _sharedSocket = NULL;

Socket * BF::Net::Socket::shared() {
	return _sharedSocket;
}

Socket::Socket() { 
	this->_cbinstream = NULL;
	this->_cbnewconn = NULL;
	this->_cbprogress = NULL;

	this->_bufferSize = 0;

	this->_connections.get().setReleaseCallback(Connection::ReleaseConnection);

	this->_portnum = 0;
	memset(this->_ip4addr, 0, SOCKET_IP4_ADDR_STRLEN);

	BFLockCreate(&this->_outqlock);

	_sharedSocket = this;
}

BF::Net::Socket::~Socket() {
	this->_tidin.lock();
	List<BFThreadAsyncID>::Node * n = this->_tidin.unsafeget().first();
	for (; n != NULL; n = n->next()) {
		BFThreadAsyncDestroy(n->object());
	}
	this->_tidin.unlock();

	BFLockDestroy(&this->_outqlock);
}

Socket * BF::Net::Socket::create(const char mode, const char * ipaddr, uint16_t port, int * err) {
	Socket * result = NULL;
	int error = 0;

	switch (mode) {
		case SOCKET_MODE_SERVER:
			result = new Server;
			break;
		case SOCKET_MODE_CLIENT:
			result = new Client;
			break;
		default:
			result = NULL;
			break;
	}

	if (result == NULL) {
		error = 1;
	} else {
		strncpy(result->_ip4addr, ipaddr, SOCKET_IP4_ADDR_STRLEN);
		result->_portnum = port;
	}

	if (err)
		*err = error;

	return result;
}

void BF::Net::Socket::setBufferSize(size_t size) {
	this->_bufferSize = size;
}

void BF::Net::Socket::setIncomingDataProgress(bool (* cb)(const unsigned char * buf, size_t size)) {
	this->_cbprogress = cb;
}

void BF::Net::Socket::setNewConnectionCallback(void (* cb)(BF::Net::Connection * sc)) {
	this->_cbnewconn = cb;
}

void BF::Net::Socket::setInStreamCallback(void (* cb)(BF::Net::Envelope * envelope)) {
	this->_cbinstream = cb;
}

uint16_t BF::Net::Socket::port() const {
	return this->_portnum;
}

const char * BF::Net::Socket::ipaddr() const {
	return this->_ip4addr;
}

/**
 * used for inStream
 *
 * allows us to run dedicated threads for each socket connection
 */
class InStreamTools : public Object {
public:
	BF::Net::Connection * mainConnection;
	BF::Net::Socket * socket;
};

void BF::Net::Socket::inStream(void * in) {
	InStreamTools * tools = (InStreamTools *) in; // we own memory
	Connection * sc = tools->mainConnection;
	Socket * skt = tools->socket;
	BFThreadAsyncID tid = BFThreadAsyncGetID();

	BFRetain(skt);

	sc->_isready = true;
	while (!BFThreadAsyncIsCanceled(tid) && sc->isactive()) {
		Envelope * envelope = new Envelope(sc, skt->_bufferSize);

		// receive data from connections using buffer
		//
		// this gets blocked until we receive something
		int err = sc->recvData(&envelope->_data);

		if (err) {
			const uint8_t sl = 1;
			BFNetLogDebug("%s - error returned from recvData: %d. Sleeping for %d seconds", err, sl);
			sleep(sl);
		} else if ((envelope->data()->size() == 0) && (sc->type() == SOCK_STREAM)) {
			sc->closeConnection(); // force the connection to close
		} else {
			if (skt->_cbinstream)
				skt->_cbinstream(envelope);
		}

		BFRelease(envelope);
	}

	// update the list
	skt->updateConnections();

	BFRelease(skt);
	BFRelease(tools);
}

// called by subclasses whenever they get a new connection
int BF::Net::Socket::startInStreamForConnection(BF::Net::Connection * sc) {
	if (!sc) {
		BFNetLogDebug("%s - null socket connection", __FUNCTION__);
		return 1;
	}

	InStreamTools * tools = new InStreamTools;
	tools->mainConnection = sc;
	tools->socket = this;

	BFThreadAsyncID tid = BFThreadAsync(Socket::inStream, (void *) tools);
	this->_tidin.get().add(tid);

	return 0;
}

int BF::Net::Socket::start() {
	this->_start();
	return 0;
}

bool BF::Net::Socket::isReady() const {
	if (this->_bufferSize == 0)
		return false;
	else if (this->_cbinstream == NULL)
		return false;
	else if (this->_cbnewconn == NULL)
		return false;

	return true;
}

void BF::Net::Socket::updateConnections() {
	this->_connections.lock();
	
	const int maxsize = this->_connections.unsafeget().count();
	int toDelete[maxsize];
	int size = 0;
	memset(toDelete, -1, sizeof(int) * size);

	// find indices to delete
	for (int i = 0; i < this->_connections.unsafeget().count(); i++) {
		Connection * conn = this->_connections.unsafeget().objectAtIndex(i);
		if (conn && !conn->isactive()) {
			toDelete[size++] = i;
		}
	}

	// delete those connections at these indices
	int offset = 0;
	for (int i = 0; i < size; i++) {
		int del = i - offset;
		if (this->_connections.unsafeget().deleteObjectAtIndex(toDelete[del])) {
			BFNetLogDebug("%s - Couldn't delete connection object at index %d", __FUNCTION__, del);
		} else {
			offset++;
		}
	}

	this->_connections.unlock();
}

int BF::Net::Socket::stop() {
	int error = 0;

	// shutdown connections
	this->_connections.lock();
	for (int i = 0; i < this->_connections.unsafeget().count(); i++) {
		Connection * conn = this->_connections.unsafeget().objectAtIndex(i);
		if (conn && conn->isactive()) {
			this->_connections.unsafeget().objectAtIndex(i)->closeConnection();
		}
	}
	this->_connections.unlock();

	// tell subclasses that they can stop too
	error = this->_stop();

	// stop our threads
	
	if (!error) {
		this->_tidin.lock();
		List<BFThreadAsyncID>::Node * n = this->_tidin.unsafeget().first();
		for (; n != NULL; n = n->next()) {
			error = BFThreadAsyncCancel(n->object());
			BFThreadAsyncWait(n->object());
			if (error) {
				break;
			}
		}
		this->_tidin.unlock();
	}

	return error;
}

