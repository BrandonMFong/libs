/**
 * author: Brando
 * date: 6/30/22
 */

#ifndef COREUTILS_TESTS_H
#define COREUTILS_TESTS_H

#include "clib_tests.h"
#include <coreutils.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

BFTEST_UNIT_FUNC(test_DoesStringArrayContain, 1, {
	char * array[] = {"Hello", "world", "we", "are", "clib"};
	int size = sizeof(array) / sizeof(array[0]);

	const char * string = "Hello";
	bool value = BFArrayStringContainsString(array, size, string);
	BF_ASSERT(value, "Array should have contained: %s", string);

	string = "tree";
	value = BFArrayStringContainsString(array, size, string);
	BF_ASSERT(!value, "Array should not have: %s", string);
})

BFTEST_UNIT_FUNC(test_GetByteStringRepresentationUsingKilo, 1, {
	char buf[20];

	int err = BFByteGetString(1000 * 1000, 0, buf);
	BF_ASSERT(err == 0);

	const char * expected = "1.00 MB";
	BF_ASSERT(!strcmp(expected, buf), "%s != %s", expected, buf);
})

BFTEST_UNIT_FUNC(test_GetByteStringRepresentationUsingKibi, 1, {
	char buf[20];

	int err = BFByteGetString(1024 * 1024, 1, buf);
	BF_ASSERT(err == 0);

	const char * expected = "1.00 MiB";
	BF_ASSERT(!strcmp(expected, buf), "%s != %s", expected, buf);
})

BFTEST_UNIT_FUNC(test_CreateBinaryStringFromNumber, 1, {
	char * string = 0;
	unsigned char a = 0x00;

	a = 0x0f;

	string = BFStringCreateBinaryStringForNumber(a, sizeof(unsigned char), &result);

	if (string == 0) {
		result = 101;
	}

	if (result == 0) {
		result = strcmp(string, "00001111");
		free(string);
	}

	if (result == 0) {
		a = 0xf0;
		string = BFStringCreateBinaryStringForNumber(a, sizeof(unsigned char), &result);
	}

	if (string == 0) {
		result = 102;
	}

	if (result == 0) {
		result = strcmp(string, "11110000");
		free(string);
	}
})

//int test_IndexOfStringInArray() {
BFTEST_UNIT_FUNC(test_IndexOfStringInArray, 1, {
	int index = 0;
	char * arr[] = {"hello", "world", "my", "name", "is", "lib"};

	index = BFArrayStringGetIndexForString(arr, 6, "my");
	if (index != 2) {
		result = 1;
		printf("1: index returned was %d\n", index);
	}

	if (!result) {
		index = BFArrayStringGetIndexForString(arr, 6, "is");
		if (index != 4) {
			result = 1;
			printf("2: index returned was %d\n", index);
		}
	}

	if (!result) {
		index = BFArrayStringGetIndexForString(arr, 6, "xpro");
		if (index != -1) {
			result = 1;
			printf("3: index returned was %d\n", index);
		}
	}
})

BFTEST_COVERAGE_FUNC(coreutils_tests) {
	BFTEST_COVERAGE_START;

	BFTEST_LAUNCH(test_DoesStringArrayContain);
	BFTEST_LAUNCH(test_CreateBinaryStringFromNumber);
	BFTEST_LAUNCH(test_IndexOfStringInArray);
	BFTEST_LAUNCH(test_GetByteStringRepresentationUsingKibi);
	BFTEST_LAUNCH(test_GetByteStringRepresentationUsingKilo);
	
	BFTEST_COVERAGE_END;
}

#endif // COREUTILS_TESTS_H

