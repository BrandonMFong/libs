/**
 * author: brando
 * date: 6/26/24
 */

#include "data.hpp"
#include "string.hpp"
#include "release.hpp"
#include "url.hpp"
#include <unistd.h>
#include <fcntl.h>

extern "C" {
#include <bflibc/bflibc.h>
#include <string.h>
}

using namespace BF;

const char * Data::className() const {
	return "BF::Data";
}

Data * Data::fromFile(const URL & url) {
	return Data::fromFile(url.abspath());
}

Data * Data::fromFile(const char * path) {
	if (!path) return NULL;
	if (!BFFileSystemPathIsFile(path)) return NULL;
	
	int err = 0;
	unsigned long long fileSize = BFFileSystemFileGetSizeUsed(path, 0, &err);
	if (err) {
		return NULL;
	}

	int fd = open(path, O_RDONLY);
	if (fd == -1) return NULL;

	Data * res = new Data(fileSize);
	if (!res) return NULL;

	const size_t bufsize = 1024;
	char buf[bufsize];
	size_t bytesRead = 0, offset = 0;
	while ((bytesRead = read(fd, buf, bufsize)) > 0) {
		if (!memcpy(((unsigned char *) res->buffer()) + offset, buf, bytesRead)) {
			close(fd);
			BFRelease(res);
			return NULL;
		}

		offset += bytesRead;
	}

	close(fd);
	return res;
}

Data::Data() : Data(0, 0) { }

Data::Data(const Data & in) : Data(in.size(), (unsigned char *) in.buffer()) { }

Data::~Data() { }

Data::Data(const size_t size, const unsigned char * data) : Array<unsigned char, size_t>() {
	this->alloc(size, data);
}

Data::Data(const String & str) : Data(str.length() + 1, (const unsigned char *) str.cString()) { }

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

int Data::compare(const Data & in) const {
	if (this->size() < in.size())
		return -1;
	else if (this->size() > in.size())
		return 1;
	
	return memcmp(
		this->buffer(),
		in.buffer(),
		this->size()
	);
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

bool Data::operator==(const Data & d) {
	return this->compare(d) == 0;
}

