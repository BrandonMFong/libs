/**
 * author: Brando
 * date: 6/30/22
 */

#ifndef FILESYSTEM_TESTS_H
#define FILESYSTEM_TESTS_H

#include "clib_tests.h"
#include <filesystem.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int test_HomePath(void) {
	int result = 0;
	char * home = BFFileSystemPathCopyHomePath(&result);

	if (result) {
		printf("CopyHomePath returned: %d\n", result);
	} else if (home == 0) {
		result = 1;
		printf("Home path is null\n");
	} else if (!BFFileSystemPathIsDirectory(home)) {
		result = 1;
		printf("Home path does not exist: %s\n", home);
	}

	free(home);

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_CalculateSizeForAvailability(void) {
	int result = 0;

	char * home = BFFileSystemPathCopyHomePath(&result);
	if (result) {
		printf("CopyHomePath returned: %d\n", result);
	}

	if (!result) {
		// We do not care about the return value of the function
		BFFileSystemPathGetSizeAvailable(home, &result);

		if (result) {
			printf("CalculateSizeForAvailability() returned %d\n", result);
		}
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_GetFileExtensionForPath(void) {
	int result = 0;
	char buf[100];

	result = BFFileSystemPathGetExtension("test.txt", buf);

	if (result == 0) {
		result = strcmp(buf, "txt");
	}

	strcpy(buf, "");
	if (result == 0) {
		result = BFFileSystemPathGetExtension("test", buf);
	}

	if (result == 0) {
		result = strcmp(buf, "");
	}

	if (result == 0) {
		result = BFFileSystemPathGetExtension("test.", buf);
	}

	if (result == 0) {
		result = strcmp(buf, "");
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_MoveFSItems(void) {
	int result = 0;
	PRINT_TEST_RESULTS(!result);
	return result;
}

void filesystem_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (!test_HomePath()) p++;
	else f++;

	if (!test_CalculateSizeForAvailability()) p++;
	else f++;

	if (!test_GetFileExtensionForPath()) p++;
	else f++;

	if (!test_MoveFSItems()) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // CLIB_TESTS_H

