/**
 * author: Brando
 * date: 10/6/22
 */

#include "string.hpp"
#include <bflibc/bflibc.h>

using namespace BF;

String::String(const String & str) : String(str.cString()) {}

String:: String() : String("") {}

String::String(const char * str) : String((char *) str) {}

String::String(char * str) : Array<char, size_t>() {
	this->set(str, strlen(str) + 1);
}

String::~String() {}

const char * String::cString() const {
	return (const char *) this->address();
}

int String::copy(String & s) const {
	s.copyFromArray(this);
	return this->count() == s.count() ? 0 : 1;
}

int String::addChar(char c) {
	return this->insertObjectAtIndex(c, this->count() - 1);
}

int String::remChar() {
	return this->removeObjectAtIndex(this->count() - 2);
}

int String::clear() {
	char str[2];
	strncpy(str, "", 2);
	this->set(str, strlen(str) + 1);
	return 0;
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

bool String::operator<(const String & s) {
	return this->compareString(s) < 0;
}

bool String::operator>(const String & s) {
	return this->compareString(s) >0;
}

int String::compareString(const String & s) const {
	return strcmp(this->cString(), s.cString());
}

size_t String::length() const {
	return this->count() - 1;
}

String & String::operator=(const String & str) {
	str.copy(*this);
	return *this;
}

