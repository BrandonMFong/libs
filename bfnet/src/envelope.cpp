/**
 * author: brando
 * date: 3/13/24
 */

#include "envelope.hpp"
#include "connection.hpp"
#include "buffer.hpp"
#include <bflibcpp/bflibcpp.hpp>

BF::Net::SocketEnvelope::SocketEnvelope(SocketConnection * sc, size_t bufsize) : _buf(0, bufsize) {
	this->_sc = sc;
	BFRetain(this->_sc);
}

BF::Net::SocketEnvelope::~SocketEnvelope() {
	BFRelease(this->_sc);
}

SocketBuffer * BF::Net::SocketEnvelope::buf() {
	return &this->_buf;
}

SocketConnection * BF::Net::SocketEnvelope::connection() {
	return this->_sc;
}

