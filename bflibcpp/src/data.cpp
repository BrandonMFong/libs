/**
 * author: brando
 * date: 6/26/24
 */

#include "data.hpp"

extern "C" {
#include <bflibc/bflibc.h>
#include <string.h>
}

using namespace BF;

Data::Data() : Data(0, 0) {

}

Data::~Data() {
}

Data::Data(const size_t size, const unsigned char * data) : Array<unsigned char, size_t>() {
	this->alloc(size, data);
}

void * Data::buffer() {
	return this->address();
}

size_t Data::size() {
	return this->count();
}

void Data::clear() {
	memset(this->buffer(), 0, this->count());
}

void Data::adjustSize(size_t s) {
	this->adjustMemorySize(s);
}

int Data::alloc(const size_t size, const unsigned char * data) {
	const unsigned char * bytes = data;
	unsigned char * dbytes = NULL;

	// create random bytes if the caller wants a buffer with
	// a size without bytes
	if ((bytes == NULL) && (size > 0)) {
		bytes = dbytes = (unsigned char *) malloc(size);
	}

	this->set(bytes, size);

	BFFree(dbytes);

	return 0;
}

