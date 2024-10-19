/**
 * author: brando
 * date: 2/26/24
 */

#include "connection.hpp"
#include "socket.hpp"
#include "envelope.hpp"
#include "buffer.hpp"
#include <bflibcpp/bflibcpp.hpp>
#include <netinet/in.h> //structure for storing address information 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/socket.h> //for socket APIs 
#include <sys/types.h> 
#include <unistd.h>
#include "internal/log.hpp"

using namespace BF;

void BF::Net::SocketConnection::ReleaseConnection(SocketConnection * sc) {
	BFRelease(sc);
}

BF::Net::SocketConnection::SocketConnection(int sd, Socket * sktref) : Object() {
	this->_sd = sd;
	this->_sktref = sktref;
	BFRetain(this->_sktref);
	uuid_generate_random(this->_uuid);
}

BF::Net::SocketConnection::~SocketConnection() {
	BFRelease(this->_sktref);
}

void BF::Net::SocketConnection::closeConnection() {
	if (shutdown(this->_sd, SHUT_RDWR) == -1) {
		BFNetLogDebug("%s - shutdown returned %d", __FUNCTION__, errno);
	}

	if (close(this->_sd) == -1) {
		BFNetLogDebug("%s - close returned %d", __FUNCTION__, errno);
	}
}

bool BF::Net::SocketConnection::isready() {
	return this->_isready.get();
}

const char BF::Net::SocketConnection::mode() {
	return this->_sktref->mode();
}

void BF::Net::SocketConnection::getuuid(uuid_t uuid) {
	memcpy(uuid, this->_uuid, sizeof(uuid_t));
}

int BF::Net::SocketConnection::queueData(const void * data, size_t size) {
	if (!data) return -2;

	// make envelope
	SocketBuffer buf(data, size);

	// queue up envelope
	int error = this->sendData(&buf);

	return error;
}

int BF::Net::SocketConnection::sendData(const SocketBuffer * buf) {
	if (!buf)
		return 1;

	BFNetLogDebug("> sendData");

	size_t bytesSent = 0;
	while (bytesSent < this->_sktref->_bufferSize) {
		size_t bytes = send(
			this->_sd,
			((unsigned char *) buf->data()) + bytesSent,
			buf->size() - bytesSent,
			0);
		if ((int) bytes == -1) {
			BFNetLogDebug("%s - errno=%d", __FUNCTION__, errno);
			return errno;
		}

		bytesSent += bytes;
		BFNetLogDebug("sent %ld/%ld bytes",
			bytesSent,
			this->_sktref->_bufferSize);
	}

	BFNetLogDebug("< sendData");

	return 0;
}

int BF::Net::SocketConnection::recvData(SocketBuffer * buf) {
	if (!buf)
		return 1;
	
	BFNetLogDebug("> recvData");

	size_t bytesReceived = 0;
	while (bytesReceived < this->_sktref->_bufferSize) {
		size_t bytes = recv(
			this->_sd,
			((unsigned char *) buf->_data) + bytesReceived,
			this->_sktref->_bufferSize - bytesReceived,
			0);
		if ((int) bytes == -1) {
			BFNetLogDebug("%s - errno=%d", __FUNCTION__, errno);
			return errno;
		} else if (bytes == 0) {
			BFNetLogDebug("%s - received 0 bytes", __FUNCTION__);
			break;
		}

		bytesReceived += bytes;
		BFNetLogDebug("received %ld/%ld bytes",
			bytesReceived,
			this->_sktref->_bufferSize);
	}

	buf->_size = bytesReceived;

	BFNetLogDebug("< recvData");

	return 0;
}

