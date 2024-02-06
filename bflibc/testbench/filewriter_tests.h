/**
 * author: Brando
 * date: 2/5/24
 */

#ifndef FILE_WRITER_TESTS_H
#define FILE_WRITER_TESTS_H

#include "clib_tests.h"
#include "filewriter.h"
#include "filesystem.h"
#include "thread.h"

#define FILE_WRITER_FILE_PATH "/tmp/filewriter_test.txt"

int test_creatingfilewriter(void) {
	UNIT_TEST_START;
	int result = 0;

	int max = 2 << 14;
	while (!result && max) {
		if (BFFileSystemPathExists(FILE_WRITER_FILE_PATH)) {
			remove(FILE_WRITER_FILE_PATH);
		}

		FileWriter fw;
		result = FileWriterCreate(&fw, FILE_WRITER_FILE_PATH);

		if (!result) {
			result = FileWriterClose(&fw);
		}

		max--;
	}


	UNIT_TEST_END(!result, result);
	return result;
}

int test_writingwithfilewriter(void) {
	UNIT_TEST_START;
	int result = 0;

	int max = 1;
	while (!result && max) {
		if (BFFileSystemPathExists(FILE_WRITER_FILE_PATH)) {
			remove(FILE_WRITER_FILE_PATH);
		}

		FileWriter fw;
		result = FileWriterCreate(&fw, FILE_WRITER_FILE_PATH);

		// write test lines
		const int lines = 2;
		if (!result) {
			for (int i = 0; i < lines; i++) {
				char line[512];
				snprintf(line, 512, "line %d", i);
				result = FileWriterQueueLine(&fw, line);

				if (result) break;
			}

			if (!result)
				FileWriterFlush(&fw);
		}

		FILE * f = 0;
		if (!result) {
			f = fopen(FILE_WRITER_FILE_PATH, "r");
			if (!f) result = max;
		}

		// read line by line and compare
		if (!result) {
			size_t s = 512;
			char * line0 = 0, line1[s];
			int i = 0;
			while (!result && (getline(&line0, &s, f) != -1)) {
				snprintf(line1, s, "line %d\n", i);

				if (strcmp(line0, line1)) {
					result = max;
				}
				
				i++;
			}
			free(line0);
		}

		fclose(f);
		if (!result) {
			result = FileWriterClose(&fw);
		}

		max--;
	}

	UNIT_TEST_END(!result, result);
	return result;
}

typedef struct {
	FileWriter * fw;
	int lines2write;
} TestFileWriterThreadsTools;

void TestFileWriterThreads(void * in) {
	TestFileWriterThreadsTools * tools = in;
	if (!tools) return;

	for (int i = 0; i < tools->lines2write; i++) {
		char line[512];
		snprintf(line, 512, "line %d", i);
		int error = FileWriterQueueLine(tools->fw, line);

		if (error) break;
	}

	FileWriterFlush(tools->fw);
}

int test_writingfromdifferentthreads(void) {
	UNIT_TEST_START;
	int result = 0;

	int max = 2 << 8;
	while (!result && max) {
		if (BFFileSystemPathExists(FILE_WRITER_FILE_PATH)) {
			remove(FILE_WRITER_FILE_PATH);
		}

		FileWriter fw;
		result = FileWriterCreate(&fw, FILE_WRITER_FILE_PATH);

		// Launch writing threads and wait for them to finish
		BFThreadAsyncID tid0 = 0;
		BFThreadAsyncID tid1 = 0;
		TestFileWriterThreadsTools tools;
		if (!result) {
			tools.fw = &fw;
			srand(time(0));
			tools.lines2write = 2 << 10;
			tid0 = BFThreadAsync(TestFileWriterThreads, (void *) &tools);
			tid1 = BFThreadAsync(TestFileWriterThreads, (void *) &tools);

			if (BFThreadAsyncIDError(tid0)) {
				printf("\ntid0 error %d\n", BFThreadAsyncIDError(tid0));
			} else if (BFThreadAsyncIDError(tid1)) {
				printf("\ntid1 error %d\n", BFThreadAsyncIDError(tid1));
			}
			fflush(stdout);

			while (BFThreadAsyncIDIsRunning(tid0) || BFThreadAsyncIDIsRunning(tid1)) {
			}

			BFThreadAsyncIDDestroy(tid0);
			BFThreadAsyncIDDestroy(tid1);
		}

		FILE * f = 0;
		if (!result) {
			f = fopen(FILE_WRITER_FILE_PATH, "r");
			if (!f) result = max;
		}

		// read line by line and compare
		if (!result) {
			char * line = 0;
			int i = 0;
			size_t s = 0;
			while ((getline(&line, &s, f) != -1)) {
				i++;
			}
			free(line);

			if (i != (tools.lines2write * 2))
				result = 1;
		}

		fclose(f);

		if (!result) {
			result = FileWriterClose(&fw);
		}

		max--;
	}

	UNIT_TEST_END(!result, result);
	return result;
}

void filewriter_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	BFThreadResetStartedCount();
	BFThreadResetStoppedCount();

	if (BFFileSystemPathExists(FILE_WRITER_FILE_PATH)) {
		remove(FILE_WRITER_FILE_PATH);
	}

	LAUNCH_TEST(test_creatingfilewriter, p, f);
	LAUNCH_TEST(test_writingwithfilewriter, p, f);
	LAUNCH_TEST(test_writingfromdifferentthreads, p, f);

	if (BFFileSystemPathExists(FILE_WRITER_FILE_PATH)) {
		remove(FILE_WRITER_FILE_PATH);
	}

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // FILE_WRITER_TESTS_H

