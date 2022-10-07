/**
 * author: Brando
 * date: 10/5/22
 */

#include "stringutils.h"
#include <stdlib.h>
#include <string.h>

char * CopyString(const char * string, int * err) {
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

