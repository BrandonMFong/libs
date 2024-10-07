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
	shutdown(this->_sd, SHUT_RDWR);
	close(this->_sd);
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
	
	send(this->_sd, buf->data(), buf->size(), 0);

	return 0;
}

int BF::Net::SocketConnection::recvData(SocketBuffer * buf) {
	if (!buf)
		return 1;

	size_t bytesReceived = 0;
	while (bytesReceived < this->_sktref->_bufferSize) {
		buf->_size = recv(
			this->_sd,
			((unsigned char *) buf->_data) + bytesReceived,
			this->_sktref->_bufferSize - bytesReceived,
			0);
		if (buf->_size == -1) {
			return errno;
		} else if (buf->_size == 0) {
			return -1;
		}

		bytesReceived += buf->_size;
	}

	return 0;
}

