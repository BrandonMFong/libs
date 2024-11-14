/**
 * author: Brando
 * date: 6/30/22
 */

#ifndef FILESYSTEM_TESTS_H
#define FILESYSTEM_TESTS_H

#include "clib_tests.h"
#include "stringutils.h"
#include "free.h"
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
	UNIT_TEST_START;
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

	UNIT_TEST_END(!result, result);
	return result;
}

int test_CalculateSizeForAvailability(void) {
	UNIT_TEST_START;
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

	BFFree(home);

	UNIT_TEST_END(!result, result);
	return result;
}

int test_GetFileExtensionForPath(void) {
	UNIT_TEST_START;
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

	UNIT_TEST_END(!result, result);

	return result;
}

int test_tmpdir(void) {
	UNIT_TEST_START;
	int result = 0;
	char tmpdir[PATH_MAX];
	result = BFFileSystemGetOSTempDirectory(tmpdir);

	if (result == 0) {
		if (!strlen(tmpdir)) result = 2;
	}
	UNIT_TEST_END(!result, result);
	return result;
}

int test_GettingNameWithoutExtension() {
	UNIT_TEST_START;
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

	UNIT_TEST_END(!result, result);
	return result;
}

int test_GettingLeafComponent() {
	UNIT_TEST_START;
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

	UNIT_TEST_END(!result, result);
	return result;
}

int test_GettingFullname() {
	UNIT_TEST_START;
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

	UNIT_TEST_END(!result, result);
	return result;
}

int test_RemoveFullDirectory(void) {
	UNIT_TEST_START;
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
	
	UNIT_TEST_END(!result, result);
	return result;
}

int test_directoryWithAPeriod(void) {
	UNIT_TEST_START;
	int result = 0;
	char tmpdir[PATH_MAX];

	// setup
	if (BFFileSystemGetOSTempDirectory(tmpdir)) {
		result = 1;
	} else if (strcat(tmpdir, "/NetFS.Framework") == NULL) {
		result = 2;
	} else if (mkdir(tmpdir, 0700)) {
		result = 3;
	}

	// test
	if (result == 0) {
		if (!BFFileSystemPathExists(tmpdir)) {
			result = 4;
		}
	}
	
	// teardown
	if (BFFileSystemRemoveAll(tmpdir)) {
		printf("could not remove: %s\n", tmpdir);
	}
	
	UNIT_TEST_END(!result, result);
	return result;
}

TEST_COVERAGE_FUNC(filesystem_tests) {
	TEST_COVERAGE_START;

	LAUNCH_TEST(test_HomePath);
	LAUNCH_TEST(test_CalculateSizeForAvailability);
	LAUNCH_TEST(test_GetFileExtensionForPath);
	LAUNCH_TEST(test_RemoveFullDirectory);
	LAUNCH_TEST(test_tmpdir);
	LAUNCH_TEST(test_GettingNameWithoutExtension);
	LAUNCH_TEST(test_GettingFullname);
	LAUNCH_TEST(test_GettingLeafComponent);
	LAUNCH_TEST(test_directoryWithAPeriod);

	TEST_COVERAGE_END;
}

#endif // CLIB_TESTS_H

