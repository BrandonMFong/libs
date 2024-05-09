/**
 * author: Brando
 * date: 6/30/22
 */

#ifndef STRINGUTILS_TESTS_H
#define STRINGUTILS_TESTS_H

#include "clib_tests.h"
#include <stringutils.h>
#include <free.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <uuid/uuid.h>

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
	UNIT_TEST_START;
	int result = 0;

	const long lim = (long) 2 << 16;
	for (long i = 0; i < lim; i++) {
		char uuidStr[kBFStringUUIDStringLength];
		BFStringGetRandomUUIDString(uuidStr);
		uuid_t bin;

		if (strlen(uuidStr) != kBFStringUUIDStringLength) {
			printf("Length: %ld != %d\n", strlen(uuidStr), kBFStringUUIDStringLength);
			result = 1;
		} else if (uuid_parse(uuidStr, bin)) {
			result = 2;
		}

		if (result) break;
	}

	UNIT_TEST_END(!result, result);
	return result;
}

#define TEST_STRINGUTILS_RANDOM_FILE "/tmp/stringutils_tests_random_file"
#define TEST_STRINGUTILS_RANDOM_TEXT "abcdefghijklmnopqrstuvwxz"
#define TEST_STRINGUTILS_RANDOM_TEXT_OCC  (1024 * 20)

void TestStringUtilsRandomFileCreate() {
	FILE * f = fopen(TEST_STRINGUTILS_RANDOM_FILE, "w");

	const char * buf = TEST_STRINGUTILS_RANDOM_TEXT;
	int max = TEST_STRINGUTILS_RANDOM_TEXT_OCC;
	while (max--) {
		fwrite(buf, sizeof(char), strlen(buf), f);
	}

	fclose(f);
}

void TestStringUtilsRandomFileDelete() {
	remove(TEST_STRINGUTILS_RANDOM_FILE);
}

int test_readingFile(void) {
	UNIT_TEST_START;
	int result = 0;

	TestStringUtilsRandomFileCreate();

	char * buf = BFStringCreateFromFile(TEST_STRINGUTILS_RANDOM_FILE);
	int bufi = 0;
	const char * randtext = TEST_STRINGUTILS_RANDOM_TEXT;
	const size_t len = strlen(TEST_STRINGUTILS_RANDOM_TEXT);
	for (int j = 0; j < TEST_STRINGUTILS_RANDOM_TEXT_OCC; j++) {
		for (int i = 0; i < len; i++) {
			if (bufi >= strlen(buf)) {
				printf("\nbufi >= strlen buf\n");
				result = 1;
				goto endloop;
			} else {
				if (buf[bufi] != randtext[i]) {
					printf("\n bufi=%d, i=%d, %c != %c\n",
							bufi, i, buf[bufi], randtext[i]);
					result = 1;
					goto endloop;
				}
				bufi++;
			}
		}
	}
endloop:

	if (buf == NULL)
		result = 1;

	BFFree(buf);
	
	TestStringUtilsRandomFileDelete();

	UNIT_TEST_END(!result, result);
	return result;
}


void stringutils_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	LAUNCH_TEST(test_CopyString, p, f);
	LAUNCH_TEST(test_uuidGen, p, f);
	LAUNCH_TEST(test_readingFile, p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // STRINGUTILS_TESTS_H

