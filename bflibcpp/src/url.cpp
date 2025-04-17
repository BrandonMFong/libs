/**
 * author: brando
 * date: 4/9/25
 */

#include "url.hpp"
#include "bflibcpp.hpp"
#include <string.h>
#include <libgen.h>

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

const char * URL::className() const {
	return "BF::URL";
}

URL::URL(const String & path) : URL(path.c_str()) { }

URL::URL(const char * path) : Object() {
	memset(&this->_reserved[0], 0, PATH_MAX);
	if (path) {
		strcpy(this->_path, path);
	} else {
		memset(&this->_path[0], 0, PATH_MAX);
	}
}

URL::~URL() { }

const char * URL::path() const {
	return this->_path;
}

const char * URL::abspath() const {
	if (realpath(this->_path, this->_reserved) == NULL) {
		// if above fails, we will just use the path as is
		strcpy(this->_reserved, this->_path);
	}
	return this->_reserved;
}

const char * URL::leaf() const {
	this->_reserved[0] = '\0';
	BFFileSystemPathGetFullname(this->_path, this->_reserved);
	return this->_reserved;
}

const char * URL::extension() const {
	this->_reserved[0] = '\0';
	if (strlen(this->_path)) {
		BFFileSystemPathGetExtension(this->_path, this->_reserved);
	}

	return this->_reserved;
}

const char * URL::name() const {
	this->_reserved[0] = '\0';
	BFFileSystemPathGetName(this->_path, this->_reserved);
	return this->_reserved;
}

const char * URL::directory() const {
	this->_reserved[0] = '\0';
	char t1[PATH_MAX];
	strcpy(t1, this->abspath());
	char * t2 = dirname(t1);

	if (t2) strcpy(this->_reserved, t2);

	return this->_reserved;
}

void URL::append(const String & suffix) {
	this->append(suffix.c_str());
}

void URL::append(const char * suffix) {
	if (!suffix) return;
	if (suffix[0] == '/') suffix++;

	size_t len = strlen(this->_path);
	if (len + 1 == PATH_MAX) return;

	if (this->_path[len - 1] != '/') {
		this->_path[len] = '/';
		this->_path[len + 1] = '\0';
	}

	strcat(this->_path, suffix);
}

bool URL::operator==(const URL & other) const {
	return strcmp(this->_path, other._path) == 0;
}

bool URL::operator!=(const URL & other) const {
	return !(*this == other);
}

bool URL::isSubPath(const URL & parent) const {
	return false;
}

