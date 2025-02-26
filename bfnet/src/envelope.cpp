/**
 * author: brando
 * date: 3/13/24
 */

#include "envelope.hpp"
#include "connection.hpp"
#include "buffer.hpp"
#include <bflibcpp/bflibcpp.hpp>

using namespace BF;
using namespace BF::Net;

SocketEnvelope::SocketEnvelope(Connection * sc, size_t bufsize) : _buf(0, bufsize) {
	this->_sc = sc;
	BFRetain(this->_sc);
}

SocketEnvelope::~SocketEnvelope() {
	BFRelease(this->_sc);
}

SocketBuffer * SocketEnvelope::buf() {
	return &this->_buf;
}

Connection * SocketEnvelope::connection() {
	return this->_sc;
}

