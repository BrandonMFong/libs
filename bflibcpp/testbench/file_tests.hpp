/**
 * author: Brando
 * date: 6/30/22
 */

#ifndef FILE_TESTS_HPP
#define FILE_TESTS_HPP

#include <file.hpp>
#include "cpplib_tests.hpp"

using namespace BF;

int test_filePath() {
	int result = 0;
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

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_basename() {
	int result = 0;
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

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_extension() {
	int result = 0;
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
			printf("extension should be an empty string\n");
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
			printf("extension should be an empty string\n");
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
			printf("extension should be an empty string\n");
			result = 1;
		}
	}

	Delete(file);

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_directory() {
	int result = 0;
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
			printf("extension should be an empty string\n");
			result = 1;
		}
	}

	Delete(file);

	PRINT_TEST_RESULTS(!result);
	return result;
}

void file_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (!test_filePath()) p++;
	else f++;
	
	if (!(test_basename())) p++;
	else f++;

	if (!test_extension()) p++;
	else f++;

	if (!test_directory()) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // FILE_TESTS_HPP

