/**
 * author: Brando
 * date: 6/30/22
 */

#ifndef STRINGUTILS_TESTS_H
#define STRINGUTILS_TESTS_H

#include "clib_tests.h"
#include "free.h"
#include <stringutils.h>
#include <free.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <uuid/uuid.h>
#include <time.h>

//int test_CopyString(void) {
BFTEST_UNIT_FUNC(test_CopyString, 1, {
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
})

BFTEST_UNIT_FUNC(test_uuidGen, 2<<10, {
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
})

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

//int test_readingFile(void) {
BFTEST_UNIT_FUNC(test_readingFile, 1, {
	TestStringUtilsRandomFileCreate();

	char * buf = BFStringCreateFromFile(TEST_STRINGUTILS_RANDOM_FILE);
	if (!buf) {
		result = 2;
	}

	if (!result) {
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
	}
endloop:

	if (!result) {
		if (buf == NULL)
			result = 1;
	}

	BFFree(buf);
	
	TestStringUtilsRandomFileDelete();
})

BFTEST_UNIT_FUNC(test_uuidcompare, 2<<10, {
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
})

BFTEST_UNIT_FUNC(test_creatingstringfromformat, 2<<10, {
	const char * fmt = "string %d";
	srand(time(0));
	int i = rand();
	char * str = BFStringCreateFormatString(fmt, i);
	char tmpstr[512];
	snprintf(tmpstr, 512, fmt, i);
	BF_ASSERT(!strcmp(str, tmpstr), "%s != %s", str, tmpstr);

	free(str);
})

BFTEST_UNIT_FUNC(test_makingarrayfromstring, 2<<10, {
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
})

BFTEST_UNIT_FUNC(test_byteArrayToHexString, 2<<10, {
	unsigned char arr[] = {
		0x00, 0x11, 0x22, 0x33, 0x44,
		0x55, 0x66, 0x77, 0x88, 0x99,
		0xaa, 0xbb, 0xcc, 0xdd, 0xee,
		0xff
	};
	char * str = BFStringCreateStringHexFromArray(
		arr,
		sizeof(arr) / sizeof(arr[0])
	);

	if (str == NULL) {
		result = 1;
	} else if (strcmp(str, "00112233445566778899AABBCCDDEEFF")) {
		result = 2;
	}

	BFFree(str);
})

BFTEST_COVERAGE_FUNC(stringutils_tests, {
	BFTEST_LAUNCH(test_CopyString);
	BFTEST_LAUNCH(test_uuidGen);
	BFTEST_LAUNCH(test_readingFile);
	BFTEST_LAUNCH(test_creatingstringfromformat);
	BFTEST_LAUNCH(test_uuidcompare);
	BFTEST_LAUNCH(test_makingarrayfromstring);
	BFTEST_LAUNCH(test_byteArrayToHexString);
})

#endif // STRINGUTILS_TESTS_H

