/**
 * author: Brando
 * date: 10/5/22
 */

#include "stringutils.h"
#include "free.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <uuid/uuid.h>

char * BFStringCopyString(const char * string) {
	unsigned long long size = strlen(string);

	char * result = (char *) malloc(size + 1);

	if (result) {
		memset(result, 0, size + 1);
		strcpy(result, string);
	}

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

int BFStringCompareUUID(const char * uuid0, const char * uuid1) {
	return memcmp(uuid0, uuid1, kBFStringUUIDStringLength);
}

char * BFStringCreateFormatArgListString(const char * format, va_list valist) {
	va_list args0, args1;
	va_copy(args0, valist);
	va_copy(args1, valist);

	// get size
	int size = vsnprintf(0, 0, format, args0);

	// create buffer
	char * result = malloc(sizeof(char) * (size + 1));
	if (!result) return NULL;

	if (vsnprintf(result, size + 1, format, args1) < 0) {
		free(result);
		return NULL;
	}

	return result;
}

char * BFStringCreateFormatString(const char * format, ...) {
	va_list valist;
	va_start(valist, format);
	char * result = BFStringCreateFormatArgListString(format, valist);
	va_end(valist);
	return result;
}

char ** BFStringCreateArrayFromString(const char * str, size_t * size, const char * delim) {
	if (!str || !size || !delim)
		return NULL;

	*size = 0;
	char ** result = NULL;
	char * buf = BFStringCopyString(str);
	char * tok = strtok(buf, delim);
	while (tok) {
		const size_t newsize = *size + 1;
		result = (char **) realloc(result, sizeof(char *) * newsize);

		if (!result)
			break;

		char * s = BFStringCopyString(tok);
		if (s)
			result[(*size)++] = s;

		tok = strtok(0, delim);
	}

	BFFree(buf);

	return result;
}

char * BFStringCreateStringHexFromArray(unsigned char * byteArray, size_t byteArraySize, size_t * size) {
	if (!byteArray || !size) {
		return NULL;
	}

	char * result = (char *) malloc(byteArraySize * 2 + 1);
	int index = 0;
	for (int i = 0; i < byteArraySize; i++) {
		sprintf(&result[index], "%02X", byteArray[i]);
		index += 2;
	}
	
	return result;
}

