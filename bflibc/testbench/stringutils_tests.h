/**
 * author: Brando
 * date: 6/30/22
 */

#ifndef STRINGUTILS_TESTS_H
#define STRINGUTILS_TESTS_H

#include "clib_tests.h"
#include <stringutils.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int test_CopyString(void) {
	int result = 0;
	const char * string = "Hello world!";
	int error = 0;
	char * output = BFStringCopyString(string, &error);

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
	return result;
}

int test_uuidGen(void) {
	int result = 0;
	char uuidStr[kBFStringUUIDStringLength];
	BFStringGetRandomUUIDString(uuidStr);
	uuid_t bin;

	if (strlen(uuidStr) != kBFStringUUIDStringLength) {
		printf("Length: %ld != %d\n", strlen(uuidStr), kBFStringUUIDStringLength);
		result = 1;
	} else if (uuid_parse(uuidStr, bin)) {
		result = 2;
	}

	if (result) { printf("Error: %d\n", result); }

	PRINT_TEST_RESULTS(!result);
	return result;
}

void stringutils_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (!test_CopyString()) p++;
	else f++;
	
	if (!test_uuidGen()) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // STRINGUTILS_TESTS_H

