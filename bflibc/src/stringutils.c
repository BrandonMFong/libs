/**
 * author: Brando
 * date: 10/5/22
 */

#include "stringutils.h"
#include "free.h"
#include <stdlib.h>
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

