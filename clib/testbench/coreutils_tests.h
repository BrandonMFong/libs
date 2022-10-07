/**
 * author: Brando
 * date: 6/30/22
 */

#ifndef COREUTILS_TESTS_H
#define COREUTILS_TESTS_H

#include "clib_tests.h"
#include "../coreutils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int test_DoesStringArrayContain(void) {
	int result = 0;
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

int test_CreateBinaryStringFromNumber() {
	int result = 0;
	char * string = 0;
	unsigned char a = 0x00;

	a = 0x0f;

	string = CreateBinaryStringFromNumber(a, sizeof(unsigned char), &result);

	if (result == 0) {
		result = strcmp(string, "00001111");
		free(string);
	}

	if (result == 0) {
		a = 0xf0;
		string = CreateBinaryStringFromNumber(a, sizeof(unsigned char), &result);
	}

	if (result == 0) {
		result = strcmp(string, "11110000");
		free(string);
	}
	
	
	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_IndexOfStringInArray() {
	int result = 0;
	int index = 0;
	char * arr[] = {"hello", "world", "my", "name", "is", "lib"};

	index = IndexOfStringInArray(arr, 6, "my");
	if (index != 2) {
		result = 1;
		printf("1: index returned was %d\n", index);
	}

	if (!result) {
		index = IndexOfStringInArray(arr, 6, "is");
		if (index != 4) {
			result = 1;
			printf("2: index returned was %d\n", index);
		}
	}

	if (!result) {
		index = IndexOfStringInArray(arr, 6, "xpro");
		if (index != -1) {
			result = 1;
			printf("3: index returned was %d\n", index);
		}
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

void coreutils_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (!test_DoesStringArrayContain()) p++;
	else f++;

	if (!test_CreateBinaryStringFromNumber()) p++;
	else f++;

	if (!test_IndexOfStringInArray()) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // COREUTILS_TESTS_H

