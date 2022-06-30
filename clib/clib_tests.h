/**
 * author: Brando
 * date: 6/30/22
 */

#include "clib.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int test_CopyString(void) {
	int result = 0;
	const char * string = "Hello world!";
	int error = 0;
	char * output = CopyString(string, &error);

	if (error != 0) {
		printf("CopyString error code: %d\n", error);
		result = 1;
	}

	if (result == 0) {
		result = strcmp(output, string);
	}

	if (output) {
		free(output);
	}

	return 0;
}

void clib_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	if (!test_CopyString()) {
		p++;
	} else {
		f++;
	}

	if (pass) *pass += p;
	if (fail) *fail += f;
}

