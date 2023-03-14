/**
 * author: Brando
 * date: 7/28/22
 */

extern "C" {
#include <string.h>
#include <libgen.h>
#include <bflibc/bflibc.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef LINUX
#include <linux/limits.h>
#endif
#include <limits.h>
}

#include "file.hpp"

using namespace BF;

File::File(const char * path, int * err) {
	int error = err ? *err : 1;

	this->_fileHandler = NULL;

	if (!path) {
		error = 2;
	} else {
		// Get the full path string
		if (realpath(path, this->_fileReserved) == NULL) {
			// if above fails, we will just use the path as is
			strcpy(this->_fileReserved, path);
		}

		// buf will hold the input path value
		this->_path = BFStringCopyString(this->_fileReserved, &error);
	}

	if (err) *err = error;
}

File::~File() {
	BFFree(this->_path);
}

const char * File::extension() {
	strcpy(this->_fileReserved, "");
	
	if (strlen(this->path())) {
		BFFileSystemPathGetExtension(this->path(), this->_fileReserved);
	}

	return (const char *) this->_fileReserved;
}

const char * File::name() {
	strcpy(this->_fileReserved, "");
	char * t1 = NULL, * t2 = NULL, t3[PATH_MAX];

	strcpy(t3, this->path());

	if (!strlen(this->path())) {
		BFDLog("The path is empty");
	} else if ((t1 = basename(t3)) == NULL) {
		BFDLog("Could not get base name: %s", this->path());
	} else if ((t2 = strrchr(t1, '.')) == NULL) {
		BFDLog("Could not find '.' in '%s'", this->path());
	} else {
		t2[0] = '\0';
		strcpy(this->_fileReserved, t1);
	}

	return (const char *) this->_fileReserved;
}

const char * File::directory() {
	char t1[PATH_MAX];
	strcpy(t1, this->path());
	char * t2 = dirname(t1);

	if (t2) strcpy(this->_fileReserved, t2);
	else strcpy(this->_fileReserved, "");

	return (const char *) this->_fileReserved;
}

