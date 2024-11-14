/**
 * author: Brando
 * date: 6/30/22
 */

#ifndef FILE_TESTS_HPP
#define FILE_TESTS_HPP

#include <file.hpp>
#include "cpplib_tests.hpp"

using namespace BF;

//int test_filePath() {
BFTEST_UNIT_FUNC(test_filePath) {
	BFTEST_UNIT_START;
	const char * path = "test/hello/world.txt";
	File * file = new File(path, &result);

	if (!result) {
		if (strcmp(file->path(), path)) {
			result = 1;
			printf("%s != %s\n", file->path(), path);
		}
	}

	Delete(file);

	if (!result) {
		path = "";
		file = new File(path, &result);
	}

	if (!result) {
		if (strlen(file->path())) {
			printf("file should have an empty string\n");
			result = 1;
		}
	}

	Delete(file);

	if (!result) {
		path = NULL;
		file = new File(path, &result);

		if (result) {
			result = 0;
		} else {
			printf("no error occured");
			result = 1;
		}
	}

	if (!result) {
		path = file->path();

		if (strcmp(path, "")) {
			printf("the path should be an empty string\n");
			result = 1;
		}
	}

	Delete(file);

	BFTEST_UNIT_END(1, 0);
}

//int test_basename() {
BFTEST_UNIT_FUNC(test_basename) {
	BFTEST_UNIT_START;
	const char * path = "test/hello/world.txt";
	File * file = new File(path, &result);

	if (!result) {
		if (strcmp(file->name(), "world")) {
			printf("%s != 'world'\n", file->path());
			result = 1;
		}
	}

	Delete(file);

	if (!result) {
		path = "test/hello/.txt";
		file = new File(path, &result);

		if (result) {
			printf("An error occurred: %d\n", result);
		}
	}

	if (!result) {
		if (strcmp(file->name(), "")) {
			printf("The file base name should be an empty string! but we got %s\n", file->name());
			result = 1;
		}
	}

	Delete(file);
	
	if (!result) {
		file = new File(NULL, &result);

		if (result) {
			result = 0;
		} else {
			printf("no error occurred\n");
			result = 1;
		}
	}

	if (!result) {
		if (strcmp(file->name(), "")) {
			printf("the base name should be an empty string since we initialized with a null string\n");
			result = 1;
		}
	}

	Delete(file);

	BFTEST_UNIT_END(1, 0);
}

//int test_extension() {
BFTEST_UNIT_FUNC(test_extension) {
	BFTEST_UNIT_START;
	const char * path = "test/hello/world.txt";
	File * file = new File(path, &result);

	if (!result) {
		if (strcmp(file->extension(), "txt")) {
			printf("%s != 'txt'\n", file->path());
			result = 1;
		}
	}

	Delete(file);

	if (!result) {
		path = "test/hello/world.";
		file = new File(path, &result);
	}

	if (!result) {
		if (strcmp(file->extension(), "")) {
			printf("extension should be an empty string but '%s'\n", file->extension());
			result = 1;
		}
	}

	Delete(file);

	if (!result) {
		path = "test/hello/world";
		file = new File(path, &result);
	}

	if (!result) {
		if (strcmp(file->extension(), "")) {
			printf("extension should be an empty string but '%s'\n", file->extension());
			result = 1;
		}
	}

	Delete(file);

	if (!result) {
		file = new File(NULL, &result);

		if (result) result = 0;
		else {
			printf("received no error\n");
			result = 1;
		}
	}

	if (!result) {
		if (strcmp(file->extension(), "")) {
			printf("extension should be an empty string but '%s'\n", file->extension());
			result = 1;
		}
	}

	Delete(file);

	BFTEST_UNIT_END(1, 0);
}

//int test_directory() {
BFTEST_UNIT_FUNC(test_directory) {
	BFTEST_UNIT_START;
	const char * path = "test/hello/world.txt";
	File * file = new File(path, &result);

	if (!result) {
		if (strcmp(file->directory(), "test/hello")) {
			printf("%s != 'test/hello'\n", file->directory());
			result = 1;
		}
	}

	Delete(file);

	if (!result) {
		path = "test/hello/world.";
		file = new File(path, &result);
	}

	if (!result) {
		if (strcmp(file->directory(), "test/hello")) {
			printf("actual: %s\n", file->directory());
			result = 1;
		}
	}

	Delete(file);

	if (!result) {
		path = "test.txt";
		file = new File(path, &result);
	}

	if (!result) {
		if (strcmp(file->directory(), ".")) {
			printf("actual: %s\n", file->directory());
			result = 1;
		}
	}

	Delete(file);

	if (!result) {
		file = new File(NULL, &result);

		if (result) result = 0;
		else {
			printf("received no error\n");
			result = 1;
		}
	}

	if (!result) {
		if (strcmp(file->extension(), "")) {
			printf("extension should be an empty string but '%s'\n", file->extension());
			result = 1;
		}
	}

	Delete(file);

	BFTEST_UNIT_END(1, 0);
}

BFTEST_COVERAGE_FUNC(file_tests) {
	BFTEST_COVERAGE_START;

	BFTEST_LAUNCH(test_filePath);
	BFTEST_LAUNCH(test_basename);
	BFTEST_LAUNCH(test_extension);
	BFTEST_LAUNCH(test_directory);

	BFTEST_COVERAGE_END;
}

#endif // FILE_TESTS_HPP

