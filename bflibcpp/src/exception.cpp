/**
 * author: brando
 * date: 7/11/24
 */

#include "exception.hpp"

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

Exception::Exception(const char * format, ...) {
	va_list valist;
	va_start(valist, format);
	this->_msg = BFStringCreateFormatArgListString(format, valist);
	va_end(valist);
}

Exception::~Exception() {
	BFFree(this->_msg);
}

const char * Exception::what() const throw() {
	return this->_msg;
}

