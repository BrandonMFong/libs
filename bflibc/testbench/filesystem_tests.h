/**
 * author: Brando
 * date: 6/30/22
 */

#ifndef FILESYSTEM_TESTS_H
#define FILESYSTEM_TESTS_H

#include "clib_tests.h"
#include "stringutils.h"
#include <filesystem.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>

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

int test_tmpdir(void) {
	int result = 0;
	char tmpdir[PATH_MAX];
	result = BFFileSystemGetOSTempDirectory(tmpdir);

	if (result == 0) {
		if (!strlen(tmpdir)) result = 2;
	}
	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_GettingNameWithoutExtension() {
	int result = 0;
	char buf[PATH_MAX];

	for (int i = 0; i < (2 << 4); i++) {
		char path[PATH_MAX];
		const char * tname = "name";
		sprintf(path, "/text/path/%s%d.txt", tname, i);
		result = BFFileSystemPathGetName(path, buf);
		if (result == 0) {
			char tmp[PATH_MAX];
			sprintf(tmp, "%s%d", tname, i);
			if (strcmp(buf, tmp)) {
				result = 1;
				break;
			}
		}
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_GettingLeafComponent() {
	int result = 0;
	char buf[PATH_MAX];

	for (int i = 0; i < (2 << 4); i++) {
		char path[PATH_MAX];
		const char * tname = "name";
		sprintf(path, "/text/path/%s%d", tname, i);
		result = BFFileSystemPathGetName(path, buf);
		if (result == 0) {
			char tmp[PATH_MAX];
			sprintf(tmp, "%s%d", tname, i);
			if (strcmp(buf, tmp)) {
				result = 1;
				break;
			}
		}
	}

	PRINT_TEST_RESULTS(!result);
	return result;

}

int test_GettingFullname() {
	int result = 0;
	char buf[PATH_MAX];

	for (int i = 0; i < (2 << 4); i++) {
		char path[PATH_MAX];
		const char * tname = "name";
		sprintf(path, "/text/path/%s%d.txt", tname, i);
		result = BFFileSystemPathGetFullname(path, buf);
		if (result == 0) {
			char tmp[PATH_MAX];
			sprintf(tmp, "%s%d.txt", tname, i);
			if (strcmp(buf, tmp)) {
				result = 1;
				break;
			}
		}
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_RemoveFullDirectory(void) {
	int result = 0;
	char tmpdir[PATH_MAX];
	char file[PATH_MAX];

	// setup
	if (BFFileSystemGetOSTempDirectory(tmpdir)) {
		result = 1;
	} else if (strcat(tmpdir, "/.test_MoveFSItems") == NULL) {
		result = 2;
	} else if (mkdir(tmpdir, 0700)) {
		result = 3;
	}

	// test
	if (result == 0) {
		// create test files
		for (int i = 0; i < 10; i++) {
			// create file path
			strcpy(file, tmpdir);
			strcat(file, "/file");
			size_t size = strlen(file);
			file[size] = BFStringIntegerToChar(i);
			file[size + 1] = '\0';
			
			// Create file
			int f = open(file, O_WRONLY | O_CREAT, S_IRWXU);

			// Write random stuff into it
			size = 2 << 8;
			char * buf = (char *) malloc(size);
			write(f, buf, size);

			// clean up
			close(f);
			free(buf);
		}
	}
	
	// teardown

	if (BFFileSystemRemoveAll(tmpdir)) {
		printf("could not remove: %s\n", tmpdir);
	}
	
	PRINT_TEST_RESULTS(!result);
	return result;
}

void filesystem_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	LAUNCH_TEST(test_HomePath, p, f);
	LAUNCH_TEST(test_CalculateSizeForAvailability, p, f);
	LAUNCH_TEST(test_GetFileExtensionForPath, p, f);
	LAUNCH_TEST(test_RemoveFullDirectory, p, f);
	LAUNCH_TEST(test_tmpdir, p, f);
	LAUNCH_TEST(test_GettingNameWithoutExtension, p, f);
	LAUNCH_TEST(test_GettingFullname, p, f);
	LAUNCH_TEST(test_GettingLeafComponent, p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // CLIB_TESTS_H

