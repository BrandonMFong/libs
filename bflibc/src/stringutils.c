/**
 * author: Brando
 * date: 10/5/22
 */

#include "stringutils.h"
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

char * BFStringCopyFormatArgListString(const char * format, va_list valist) {
	
	return NULL;
}

char * BFStringCreateFormatString(const char * format, ...) {
	va_list valist;
	va_start(valist, format);

	// get size
	int size = vsnprintf(0, 0, format, valist);

	// create buffer
	char * result = malloc(sizeof(char) * (size + 1));
	if (!result) return NULL;

	if (vsnprintf(result, size + 1, format, valist) < 0) {
		free(result);
		return NULL;
	}

	va_end(valist);
	return result;
}

