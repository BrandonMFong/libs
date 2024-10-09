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

using namespace BF;
using namespace BF::Net;

Socket * _sharedSocket = NULL;

Socket * BF::Net::Socket::shared() {
	return _sharedSocket;
}

Socket::Socket() { 
	this->_cbinstream = NULL;
	this->_cbnewconn = NULL;

	this->_bufferSize = 0;

	this->_connections.get().setReleaseCallback(SocketConnection::ReleaseConnection);

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

void BF::Net::Socket::setNewConnectionCallback(int (* cb)(BF::Net::SocketConnection * sc)) {
	this->_cbnewconn = cb;
}

void BF::Net::Socket::setInStreamCallback(void (* cb)(BF::Net::SocketEnvelope * envelope)) {
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
	BF::Net::SocketConnection * mainConnection;
	BF::Net::Socket * socket;
};

void BF::Net::Socket::inStream(void * in) {
	InStreamTools * tools = (InStreamTools *) in; // we own memory
	SocketConnection * sc = tools->mainConnection;
	Socket * skt = tools->socket;
	BFThreadAsyncID tid = BFThreadAsyncGetID();

	BFRetain(skt);

	sc->_isready = true;	
	while (!BFThreadAsyncIsCanceled(tid)) {
		SocketEnvelope * envelope = new SocketEnvelope(sc, skt->_bufferSize);

		// receive data from connections using buffer
		//
		// this gets blocked until we receive something
		int err = sc->recvData(&envelope->_buf);
        if (!err && skt->_cbinstream) {
			skt->_cbinstream(envelope);
		}

		BFRelease(envelope);
	}

	BFRelease(skt);
	BFRelease(tools);
}

// called by subclasses whenever they get a new connection
int BF::Net::Socket::startInStreamForConnection(BF::Net::SocketConnection * sc) {
	if (!sc) return 1;

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
	return true;
}

int BF::Net::Socket::stop() {
	int error = 0;

	// shutdown connections
	this->_connections.lock();
	for (int i = 0; i < this->_connections.unsafeget().count(); i++) {
		this->_connections.unsafeget().objectAtIndex(i)->closeConnection();
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

