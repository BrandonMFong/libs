/**
 * author: Brando
 * date: 10/6/22
 */

#include "string.hpp"
#include "data.hpp"
#include <stdexcept>
#include <stdarg.h>

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

const char * String::className() const {
	return "BF::String";
}

String::String(const String & str) : String(str.cString()) {}

String:: String() : String("") {}

String::String(const std::string & str) : String(str.c_str()) { }

String::String(char * str) : Array<char, size_t>() {
	this->set(str, strlen(str) + 1);
}

String::String(long int nullstr) : String((int) nullstr) {}

String::String(int nullstr) {
	if (nullstr != 0) throw std::invalid_argument("cannot set BF::String object to a nonzero integer");

	this->set((char *) "", 1);
}

String::String(const Data & data) : Array<char, size_t>() {
	this->set((const char *) data.buffer(), data.size());

	/**
	 * if the last element in the array is not the 
	 * null terminating character, then I will
	 * adjust the array add the null terminator
	 */
	if (this->address()[this->count() - 1] != '\0') {
		this->adjustMemorySize(this->count() + 1);
		this->addChar('\0');
	}
}

String::String(const char * format, va_list valist) : Array<char, size_t>() {
	char * str = BFStringCreateFormatArgListString(format, valist);
	this->set(str, strlen(str) + 1);
	BFFree(str);
}

String::String(const char * format, ...) : Array<char, size_t>() {
	va_list valist;
	va_start(valist, format);
	char * str = BFStringCreateFormatArgListString(format, valist);
	va_end(valist);
	this->set(str, strlen(str) + 1);
	BFFree(str);
}

String * String::createWithFormat(const char * format, ...) {
	va_list valist;
	va_start(valist, format);
	String * result = new String(format, valist);
	va_end(valist);
	return result;
}

String::~String() {}

const char * String::cString() const {
	return (const char *) this->address();
}

const char * String::c_str() const {
	return this->cString();
}

bool String::starts_with(const char * sstr) const {
	char * str = strstr(this->address(), sstr);
	return str == this->address();
}

char * String::cStringCopy() const {
	return BFStringCopyString((const char *) this->address());
}

int String::readFromFile(const char * file) {
	if (!file) return 1;
	char * buf = BFStringCreateFromFile(file);
	this->set(buf, strlen(buf) + 1);

	BFFree(buf);

	return 0;
}

int String::copy(String & s) const {
	s.copyFromArray(this);
	return this->count() == s.count() ? 0 : 1;
}

int String::addChar(char c) {
	return this->insertObjectAtIndex(c, this->count() - 1);
}

void String::push_back(char c) {
	this->addChar(c);
}

int String::addCharAtIndex(char c, size_t index) {
	if (index <= (this->count() - 1)) {
		return this->insertObjectAtIndex(c, index);
	}
	return 81;
}

int String::remChar() {
	return this->removeObjectAtIndex(this->count() - 2);
}

void String::pop_back() {
	this->remChar();
}

int String::remCharAtIndex(size_t index) {
	if (index <= (this->count() - 2)) {
		return this->removeObjectAtIndex(index);
	}

	return 80;
}

void String::append(const String & suffix) {
	this->removeObjectAtIndex(this->count() - 1);
	this->Array::append(suffix);
}

void String::append(const char * format, ...) {
	va_list va;
	va_start(va, format);
	this->append(format, va);
	va_end(va);
}

void String::append(const char * format, va_list valist) {
	String add(format, valist);
	this->append(add);
}

int String::clear() {
	char str[2];
	strncpy(str, "", 2);
	this->set(str, strlen(str) + 1);
	return 0;
}

bool String::operator==(const String & s) {
	return this->compareString(s) == 0;
}

bool String::operator==(const char * s) {
	return this->compareString(s) == 0;
}

bool String::operator!=(const char * s) {
	return this->compareString(s) != 0;
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

int String::compare(const Object & s) const {
	if (strcmp(s.className(), this->className())) {
		return this->Object::compare(s);
	}

	const String * str = (const String *) &s;
	return this->compareString(*str);
}

size_t String::length() const {
	return this->count() - 1;
}

bool String::empty() const {
	return this->length() == 0;
}

String & String::operator=(const String & str) {
	str.copy(*this);
	return *this;
}

const char String::operator[](size_t index) {
	return this->objectAtIndex(index);
}

int String::toi(const String & s) {
	return atoi(s.cString());
}

