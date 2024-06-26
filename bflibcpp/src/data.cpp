/**
 * author: brando
 * date: 6/26/24
 */

#include "data.hpp"

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

Data::Data() : Data(0, 0) {

}

Data::~Data() {
}

Data::Data(const size_t size) : Data(size, 0) {

}

Data::Data(const size_t size, const unsigned char * data) : Array<unsigned char, size_t>() {
	const unsigned char * bytes = data;
	unsigned char * dbytes = NULL;

	if ((bytes == NULL) && (size > 0)) {
		bytes = dbytes = (unsigned char *) malloc(size);
	}

	this->set(bytes, size);

	BFFree(dbytes);
}

void * Data::buffer() {
	return this->address();
}

