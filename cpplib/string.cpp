/**
 * author: Brando
 * date: 10/6/22
 */

#include "string.hpp"
#include "stringutils.h"
#include <string.h>

String:: String() : String("") {

}

String::String(const char * str) : String((char *) str) {

}

String::String(char * str) : Array<char>(str, strlen(str) + 1) {

}

String::~String() {

}

const char * String::cString() const {
	return (const char *) this->address();
}

std::ostream & operator<<(std::ostream & out, const String & s) {
	return out << s.cString();
}

String::operator const char * () const {
	return this->cString();
}

bool String::operator==(const String & s) {
	return this->compareString(s) == 0;
}

bool String::operator!=(const String & s) {
	return this->compareString(s) != 0;
}

int String::compareString(const String & s) const {
	return strcmp(this->cString(), s.cString());
}

size_t String::length() const {
	return this->count() - 1;
}

