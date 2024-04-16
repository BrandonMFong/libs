/**
 * author: brando
 * date: 3/13/24
 */

#include "buffer.hpp"
#include <bflibcpp/bflibcpp.hpp>

BF::Net::SocketBuffer::SocketBuffer(const void * data, size_t size) {
	this->_data = malloc(size);

	if (data)
		memcpy(this->_data, data, size);

	this->_size = size;
}

BF::Net::SocketBuffer::~SocketBuffer() {
	BFFree(this->_data);
}

const void * BF::Net::SocketBuffer::data() const {
	return this->_data;
}

size_t BF::Net::SocketBuffer::size() const {
	return this->_size;
}

