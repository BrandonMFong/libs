/**
 * author: brando
 * date: 2/26/24
 */

#include "connection.hpp"
#include "socket.hpp"
#include "envelope.hpp"
#include <bflibcpp/bflibcpp.hpp>
#include <netinet/in.h> //structure for storing address information 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/socket.h> //for socket APIs 
#include <sys/types.h> 
#include <unistd.h>
#include "internal/log.hpp"

using namespace BF;

void BF::Net::Connection::ReleaseConnection(Connection * sc) {
	BFRelease(sc);
}

BF::Net::Connection::Connection(int sd, Socket * sktref) : Object() {
	this->_sd = sd;
	this->_sktref = sktref;
	BFRetain(this->_sktref);
	uuid_generate_random(this->_uuid);
}

BF::Net::Connection::~Connection() {
	BFRelease(this->_sktref);
}

void BF::Net::Connection::closeConnection() {
	this->_sd.lock();
	if (shutdown(this->_sd.unsafeget(), SHUT_RDWR) == -1) {
		BFNetLogDebug("%s - shutdown returned %d", __FUNCTION__, errno);
	}

	if (close(this->_sd.unsafeget()) == -1) {
		BFNetLogDebug("%s - close returned %d", __FUNCTION__, errno);
	}

	this->_sd.unsafeset(0);
	this->_sd.unlock();
}

bool BF::Net::Connection::isready() const {
	return this->_isready.get();
}

bool BF::Net::Connection::isactive() const {
	if (this->_sd.get() == 0) {
		BFNetLogDebug("%s - socket descriptor is 0", __FUNCTION__);
		return false;
	}

	int error = 0;
	socklen_t len = sizeof(error);
	int retval = getsockopt(this->_sd.get(), SOL_SOCKET, SO_ERROR, &error, &len);

	if (retval != 0) {
		BFNetLogDebug("%s - error getting socket error code: %s", __FUNCTION__, strerror(retval));
		return false;
	}

	if (error != 0) {
		/* socket has a non zero error status */
		BFNetLogDebug("%s - socket error: %s", __FUNCTION__, strerror(error));
		return false;
	}
	
	return true;
}

const char BF::Net::Connection::mode() {
	return this->_sktref->mode();
}

void BF::Net::Connection::getuuid(uuid_t uuid) {
	memcpy(uuid, this->_uuid, sizeof(uuid_t));
}

int BF::Net::Connection::type() const {
    int type = 0;
    socklen_t length = sizeof( int );
    if (getsockopt(this->_sd.get(), SOL_SOCKET, SO_TYPE, &type, &length) == -1) {
		BFNetLogDebug("%s - couldn't get socket type errno=%d", __FUNCTION__, errno);
		return -1;
	}
	return type;
}

int BF::Net::Connection::queueData(const void * data, size_t size) {
	if (!data) return -2;

	// make envelope
	Data buf(size, (const unsigned char *) data);

	// queue up envelope
	int error = this->sendData(&buf);

	return error;
}

int BF::Net::Connection::sendData(const Data * buf) {
	if (this->_sd.get() == 0) {
		return 1;
	} else if (!buf) {
		return 1;
	}

	BFNetLogDebug("> sendData");

	int result = 0;
	size_t bytesSent = 0;
	while (bytesSent < this->_sktref->_bufferSize) {
		size_t bytes = send(
			this->_sd.get(),
			((unsigned char *) buf->buffer()) + bytesSent,
			buf->size() - bytesSent,
			0);
		if ((int) bytes == -1) {
			BFNetLogDebug("%s - errno=%d", __FUNCTION__, errno);
			result = errno;
			break;
		}

		bytesSent += bytes;
		BFNetLogDebug("sent %ld/%ld bytes",
			bytesSent,
			this->_sktref->_bufferSize);
	}

	BFNetLogDebug("< sendData");

	return result;
}

int BF::Net::Connection::recvData(Data * data) {
	if (this->_sd.get() == 0) {
		return 1;
	} else if (!data) {
		return 1;
	}

	BFNetLogDebug("> recvData");

	int result = 0;
	size_t bytesReceived = 0;
	unsigned char * buf = (unsigned char *) malloc(this->_sktref->_bufferSize);
	do {
		// read socket
		size_t bytes = recv(
			this->_sd.get(),
			buf,
			this->_sktref->_bufferSize,
			0);
		
		// handle error
		if ((int) bytes == -1) {
			BFNetLogDebug("%s - errno=%d", __FUNCTION__, errno);
			result = -1;
			break;
		} else if (bytes == 0) {
			BFNetLogDebug("%s - received 0 bytes", __FUNCTION__); // eof
			break;
		}

		// stretch the buffer if needed
		if (data->size() < (bytesReceived + bytes)) {
			data->resize(bytesReceived + bytes);
		}

		// copy to our buffer
		memcpy(((unsigned char *) data->buffer()) + bytesReceived,
				buf,
				bytes);
		bytesReceived += bytes;
		BFNetLogDebug("received %ld bytes",
			bytesReceived);

		if (this->_sktref->_cbprogress == NULL) {
			break;
		} else {
			/**
			 * will only tell the receiver about the bytes we have so far
			 *
			 * the receiver should tell us if they want us to keep
			 * trying to socket, if not then we will break
			 */
			if (this->_sktref->_cbprogress((const unsigned char *) data->buffer(), bytesReceived)) {
				break;
			}
		}
	} while (true);

	data->resize(bytesReceived);

	BFNetLogDebug("< recvData");

	return result;
}

