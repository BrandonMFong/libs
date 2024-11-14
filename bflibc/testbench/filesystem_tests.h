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

//int test_HomePath(void) {
BFTEST_UNIT_FUNC(test_HomePath, 1, {
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
})

//int test_CalculateSizeForAvailability(void) {
BFTEST_UNIT_FUNC(test_CalculateSizeForAvailability, 1, {
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
})

//int test_GetFileExtensionForPath(void) {
BFTEST_UNIT_FUNC(test_GetFileExtensionForPath, 1, {
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
})

//int test_tmpdir(void) {
BFTEST_UNIT_FUNC(test_tmpdir, 1, {
	char tmpdir[PATH_MAX];
	result = BFFileSystemGetOSTempDirectory(tmpdir);

	if (result == 0) {
		if (!strlen(tmpdir)) result = 2;
	}
})

//int test_GettingNameWithoutExtension() {
BFTEST_UNIT_FUNC(test_GettingNameWithoutExtension, 1, {
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
})

//int test_GettingLeafComponent() {
BFTEST_UNIT_FUNC(test_GettingLeafComponent, 1, {
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
})

//int test_GettingFullname() {
BFTEST_UNIT_FUNC(test_GettingFullname, 1, {
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
})

//int test_RemoveFullDirectory(void) {
BFTEST_UNIT_FUNC(test_RemoveFullDirectory, 1, {
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
})

//int test_directoryWithAPeriod(void) {
BFTEST_UNIT_FUNC(test_directoryWithAPeriod, 1, {
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
})

BFTEST_COVERAGE_FUNC(filesystem_tests) {
	BFTEST_COVERAGE_START;

	BFTEST_LAUNCH(test_HomePath);
	BFTEST_LAUNCH(test_CalculateSizeForAvailability);
	BFTEST_LAUNCH(test_GetFileExtensionForPath);
	BFTEST_LAUNCH(test_RemoveFullDirectory);
	BFTEST_LAUNCH(test_tmpdir);
	BFTEST_LAUNCH(test_GettingNameWithoutExtension);
	BFTEST_LAUNCH(test_GettingFullname);
	BFTEST_LAUNCH(test_GettingLeafComponent);
	BFTEST_LAUNCH(test_directoryWithAPeriod);

	BFTEST_COVERAGE_END;
}

#endif // CLIB_TESTS_H

