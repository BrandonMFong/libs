/**
 * author: Brando
 * date: 10/5/22
 */

#include "stringutils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <uuid/uuid.h>

char * BFStringCopyString(const char * string, int * err) {
	int error = 0;
	unsigned long long size = strlen(string);

	char * result = (char *) malloc(size + 1);

	if (result) {
		memset(result, 0, size + 1);
		strcpy(result, string);
	} else {
		error = 1;
	}

	if (err != 0) *err = error;

	return result;
}

void BFStringGetRandomUUIDString(char * uuidString) {
	uuid_t bin;

	uuid_generate_random(bin);
	uuid_unparse_lower(bin, uuidString);
}

char * BFStringCreateFromFile(const char * file) {
	if (!file)
		return NULL;

	FILE * f = fopen(file, "r");
	if (!f)
		return NULL;

	// get size of file
	fseek(f, 0, SEEK_END);
	size_t filesize = ftell(f);
	rewind(f);

	// get buf the size of the file
	char * buf = (char *) malloc(sizeof(char) * filesize);

	// we will read a certain amount bytes at a time
	char * tmp = buf; // using this to read
	size_t bytesread = 0;
	const size_t readsize = 2 << 10;
	while ((bytesread = fread(tmp, sizeof(char), readsize, f)) > 0) {
		tmp += bytesread; // shift the buffer to end of the read bytes
	}

	return buf;
}

