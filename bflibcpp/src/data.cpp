/**
 * author: brando
 * date: 6/26/24
 */

#include "data.hpp"
#include "string.hpp"

extern "C" {
#include <bflibc/bflibc.h>
#include <string.h>
}

using namespace BF;

Data::Data() : Data(0, 0) { }

Data::~Data() { }

Data::Data(const size_t size, const unsigned char * data) : Array<unsigned char, size_t>() {
	this->alloc(size, data);
}

Data::Data(String & str) : Data(str.length() + 1, (const unsigned char *) str.cString()) { }

void * Data::buffer() const {
	return this->address();
}

String Data::hex() const {
	char * buf = BFStringCreateStringHexFromArray(
		(unsigned char *) this->buffer(), this->size()
	);
	String res(buf);
	BFFree(buf);
	return res;
}

size_t Data::size() const {
	return this->count();
}

void Data::clear() {
	memset(this->buffer(), 0, this->count());
}

void Data::resize(size_t s) {
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

