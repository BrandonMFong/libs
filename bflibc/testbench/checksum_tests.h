/**
 * author: Brando
 * date: 6/2/23
 */

#ifndef CHECKSUM_TESTS_H
#define CHECKSUM_TESTS_H

#include "clib_tests.h"
#include <checksum.h>
#include <string.h>

#define kBFChecksumTestString "Hello World!"

int checksumTest(BFChecksumType type, const char * in, const char * expected) {
	int result = 0;
	
	BFChecksumTools tools;
	if (BFChecksumCreate(&tools, type)) {
		result = 1;
	} else if (BFChecksumUpdate(&tools, in, strlen(in))) {
		result = 2;
	} else if (BFChecksumFinal(&tools)) {
		result = 3;
	}

	char * dig = 0;
	if (result == 0) {
		dig = (char *) malloc(BFChecksumGetDigestStringLength(&tools) + 1);
		result = BFChecksumGetDigestString(&tools, dig);
	}

	if (result == 0) {
		if (dig == 0) {
			result = 4;
		} else if (strcmp(dig, expected)) {
			printf("Checksums do not match '%s' != '%s'\n", dig, expected);
			result = 5;
		}
	}

	free(dig);
	BFChecksumDestroy(&tools);

	return result;
}

int test_checksumMD5() {
	int result = 0;
	const char * expected = "ed076287532e86365e841e92bfc50d8c";
	result = checksumTest(kBFChecksumTypeMD5, kBFChecksumTestString, expected);
	PRINT_TEST_RESULTS(!result);

	return result;
}

int test_checksumSHA1() {
	int result = 0;
	const char * expected = "2ef7bde608ce5404e97d5f042f95f89f1c232871";
	result = checksumTest(kBFChecksumTypeSHA1, kBFChecksumTestString, expected);
	PRINT_TEST_RESULTS(!result);

	return result;
}

int test_checksumSHA256() {
	int result = 0;
	const char * expected = "7f83b1657ff1fc53b92dc18148a1d65dfc2d4b1fa3d677284addd200126d9069";
	result = checksumTest(kBFChecksumTypeSHA256, kBFChecksumTestString, expected);
	PRINT_TEST_RESULTS(!result);

	return result;
}

int test_checksumSHA512() {
	int result = 0;
	const char * expected = "861844d6704e8573fec34d967e20bcfef3d424cf48be04e6dc08f2bd58c729743371015ead891cc3cf1c9d34b49264b510751b1ff9e537937bc46b5d6ff4ecc8";
	result = checksumTest(kBFChecksumTypeSHA512, kBFChecksumTestString, expected);
	PRINT_TEST_RESULTS(!result);

	return result;
}

void checksum_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (!test_checksumMD5()) p++;
	else f++;
	
	if (!test_checksumSHA1()) p++;
	else f++;
	
	if (!test_checksumSHA256()) p++;
	else f++;

	if (!test_checksumSHA512()) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // CHECKSUM_TESTS_H

