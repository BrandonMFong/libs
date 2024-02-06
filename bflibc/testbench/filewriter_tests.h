/**
 * author: Brando
 * date: 2/5/24
 */

#ifndef FILE_WRITER_TESTS_H
#define FILE_WRITER_TESTS_H

#include "clib_tests.h"
#include "filewriter.h"

#define FILE_WRITER_FILE_PATH "/tmp/filewriter_test.txt"

int test_creatingfilewriter(void) {
	UNIT_TEST_START;
	int result = 0;

	FileWriter fw;
	result = FileWriterCreate(&fw, FILE_WRITER_FILE_PATH);

	if (!result) {
		result = FileWriterClose(&fw);
	}

	UNIT_TEST_END(!result, result);
	return result;
}

void filewriter_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	LAUNCH_TEST(test_creatingfilewriter, p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // FILE_WRITER_TESTS_H

