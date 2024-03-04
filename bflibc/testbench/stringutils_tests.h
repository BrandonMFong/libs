/**
 * author: Brando
 * date: 6/30/22
 */

#ifndef STRINGUTILS_TESTS_H
#define STRINGUTILS_TESTS_H

#include "clib_tests.h"
#include "free.h"
#include <stringutils.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int test_CopyString(void) {
	int result = 0;
	const char * string = "Hello world!";
	int error = 0;
	char * output = BFStringCopyString(string);
	if (!output)
		error = 2;

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
	UNIT_TEST_START;
	int result = 0;

	const long lim = (long) 2 << 16;
	for (long i = 0; i < lim; i++) {
		char uuidStr[kBFStringUUIDStringLength];
		BFStringGetRandomUUIDString(uuidStr);
		uuid_t bin;

		if (strlen(uuidStr) != kBFStringUUIDStringLength - 1) {
			printf("Length: %ld != %d\n", strlen(uuidStr), kBFStringUUIDStringLength - 1);
			result = 1;
		} else if (uuid_parse(uuidStr, bin)) {
			result = 2;
		}

		if (result) break;
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_uuidcompare(void) {
	UNIT_TEST_START;
	int result = 0;

	int max = 2 << 1;
	while (!result && max) {
		const long lim = (long) 2 << 16;
		for (long i = 0; i < lim; i++) {
			char uuid0[kBFStringUUIDStringLength];
			char uuid1[kBFStringUUIDStringLength];
			char uuid2[kBFStringUUIDStringLength];
			
			BFStringGetRandomUUIDString(uuid0);
			BFStringGetRandomUUIDString(uuid1);
			memcpy(uuid2, uuid1, kBFStringUUIDStringLength);
		
			uuid_t bin;

			if (strlen(uuid0) != kBFStringUUIDStringLength - 1) {
				printf("Length: %ld != %d\n", strlen(uuid0), kBFStringUUIDStringLength);
				result = 1;
			} else if (strlen(uuid1) != kBFStringUUIDStringLength - 1) {
				printf("Length: %ld != %d\n", strlen(uuid1), kBFStringUUIDStringLength);
				result = 2;
			} else if (strlen(uuid2) != kBFStringUUIDStringLength - 1) {
				printf("Length: %ld != %d\n", strlen(uuid2), kBFStringUUIDStringLength);
				result = 3;
			} else if (!BFStringCompareUUID(uuid0, uuid1)) {
				printf("%s == %s\n", uuid0, uuid1);
				result = 4;
			} else if (BFStringCompareUUID(uuid2, uuid1)) {
				printf("%s != %s\n", uuid2, uuid1);
				result = 5;
			} else if (uuid_parse(uuid0, bin)) {
				result = 6;
			} else if (uuid_parse(uuid1, bin)) {
				result = 7;
			} else if (uuid_parse(uuid2, bin)) {
				result = 8;
			}

			if (result) break;
		}

		max--;
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_creatingstringfromformat(void) {
	UNIT_TEST_START;
	int result = 0;

	int numstr = 2 << 20;
	for (int i = 0; i < numstr; i++) {
		const char * fmt = "string %d";
		char * str = BFStringCreateFormatString(fmt, i);
		char tmpstr[512];
		snprintf(tmpstr, 512, fmt, i);
		if (strcmp(str, tmpstr)) {
			result = i;
			break;
		}

		free(str);
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_makingarrayfromstring() {
	UNIT_TEST_START;
	int result = 0;

	int max = 2 << 20;
	while (!result && max) {
		const char * str = "hello world this is a test";
		char * exp[] = {"hello", "world", "this", "is", "a", "test"};
		size_t expsize = (sizeof(exp) / sizeof(exp[0]));
		const char * delim = " ";
		size_t size = 0;
		char ** arr = BFStringCreateArrayFromString(str, &size, delim);
		if (!arr)
			result = max;

		if (!result) {
			if (size != expsize) {
				result = max;
				printf("\nsize %ld != %ld\n", size, expsize);
			} else {
				for (int i = 0; i < size; i++) {
					if (strcmp(arr[i], exp[i])) {
						printf("\n%s != %s\n", arr[i], exp[i]);
						result = max;
						break;
					}
				}
			}
		}

		if (arr) {
			for (int i = 0; i < size; i++) {
				BFFree(arr[i]);
			}
			BFFree(arr);
		}

		max--;
	}

	UNIT_TEST_END(!result, result);
	return result;
}

void stringutils_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	LAUNCH_TEST(test_CopyString, p, f);
	LAUNCH_TEST(test_uuidGen, p, f);
	LAUNCH_TEST(test_creatingstringfromformat, p, f);
	LAUNCH_TEST(test_uuidcompare, p, f);
	LAUNCH_TEST(test_makingarrayfromstring, p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // STRINGUTILS_TESTS_H

