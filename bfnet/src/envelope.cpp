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

Envelope::Envelope(Connection * sc, size_t bufsize) : _buf(0, bufsize) {
	this->_sc = sc;
	BFRetain(this->_sc);
}

Envelope::~Envelope() {
	BFRelease(this->_sc);
}

SocketBuffer * Envelope::buf() {
	return &this->_buf;
}

Connection * Envelope::connection() {
	return this->_sc;
}

