/**
 * author: brando
 * date: 3/13/24
 */

#include "envelope.hpp"
#include "connection.hpp"
#include <bflibcpp/bflibcpp.hpp>

using namespace BF;
using namespace BF::Net;

Envelope::Envelope(Connection * sc, size_t bufsize) : _data(bufsize, 0) {
	this->_sc = sc;
	BFRetain(this->_sc);
}

Envelope::~Envelope() {
	BFRelease(this->_sc);
}

Data * Envelope::data() {
	return &this->_data;
}

Connection * Envelope::connection() {
	return this->_sc;
}

