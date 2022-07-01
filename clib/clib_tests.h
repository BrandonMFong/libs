/**
 * author: Brando
 * date: 6/30/22
 */

#ifdef TESTING

#include "tests.h"
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

	PRINT_TEST_RESULTS(!result);

	return result;;
}

int test_DoesStringArrayContain(void) {
	int result;
	char * array[] = {"Hello", "world", "we", "are", "clib"};
	int size = sizeof(array) / sizeof(array[0]);

	const char * string = "Hello";
	bool value = DoesStringArrayContain(array, size, string);
	
	if (!value) {
		printf("Array should have contained: %s\n", string);
		result = 1;
	}

	if (result == 0) {
		string = "tree";
		value = DoesStringArrayContain(array, size, string);

		if (value) {
			result = 1;
			printf("Array should not have: %s\n", string);
		}
	}

	PRINT_TEST_RESULTS(!result);

	return result;
}

int test_GetByteStringRepresentation(void) {
	int result = 0;
	char buf[20];

	result = GetByteStringRepresentation(1024 * 1024, buf);

	const char * expected = "1.00 mb";

	if (result == 0) {
		if (strcmp(expected, buf)) {
			result = 1;
			printf("%s != %s\n", expected, buf);
		}
	}

	PRINT_TEST_RESULTS(!result);

	return result;

}

void clib_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (!test_CopyString()) p++;
	else f++;

	if (!test_DoesStringArrayContain()) p++;
	else f++;

	if (!test_GetByteStringRepresentation()) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // TESTING

