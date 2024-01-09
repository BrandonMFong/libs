/**
 * author: brando
 * date: 1/5/24
 */

#include "path.hpp"
#include <string.h>
#include <stdlib.h>

using namespace BF;

Path::Path(const char * path) {
	memset(&this->_path[0], 0, PATH_MAX);
	memset(&this->_reserved[0], 0, PATH_MAX);
	if (path) {
		strcpy(this->_path, path);
	}
}

Path::~Path() {

}

const char * Path::path() {
	if (this->_path) {
		return this->_path;
	} else {
		return "";
	}
}

const char * Path::abspath() {
	if (realpath(this->_path, this->_reserved) == NULL) {
		// if above fails, we will just use the path as is
		strcpy(this->_reserved, this->_path);
	}
	return this->_reserved;
}

