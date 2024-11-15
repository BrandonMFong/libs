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

BFTEST_UNIT_FUNC(test_creatingfilewriter, 2 << 4, {
	if (BFFileSystemPathExists(FILE_WRITER_FILE_PATH)) {
		remove(FILE_WRITER_FILE_PATH);
	}

	BFFileWriter fw;
	result = BFFileWriterCreate(&fw, FILE_WRITER_FILE_PATH);

	if (!result) {
		result = BFFileWriterClose(&fw);
	}
})

BFTEST_UNIT_FUNC(test_writingwithfilewriter, 2<<4, {
	if (BFFileSystemPathExists(FILE_WRITER_FILE_PATH)) {
		remove(FILE_WRITER_FILE_PATH);
	}

	BFFileWriter fw;
	result = BFFileWriterCreate(&fw, FILE_WRITER_FILE_PATH);

	// write test lines
	const int lines = 2 << 4;
	if (!result) {
		for (int i = 0; i < lines; i++) {
			char line[512];
			snprintf(line, 512, "line %d", i);
			result = BFFileWriterQueueLine(&fw, line);

			if (result) break;
		}

		if (!result)
			BFFileWriterFlush(&fw);
	}

	FILE * f = 0;
	if (!result) {
		f = fopen(FILE_WRITER_FILE_PATH, "r");
		if (!f) result = max + 1000;
	}

	// read line by line and compare
	if (!result) {
		size_t s = 512;
		char * line0 = 0, line1[s];
		int i = 0;
		while (!result && (getline(&line0, &s, f) != -1)) {
			snprintf(line1, s, "line %d\n", i);

			if (strcmp(line0, line1)) {
				result = max + 1000;
			}
			
			i++;
		}
		free(line0);
	}

	fclose(f);
	if (!result) {
		result = BFFileWriterClose(&fw);
	}
})

typedef struct {
	BFFileWriter * fw;
	int lines2write;
} TestFileWriterThreadsTools;

void TestFileWriterThreads(void * in) {
	TestFileWriterThreadsTools * tools = in;
	if (!tools) return;

	for (int i = 0; i < tools->lines2write; i++) {
		char line[512];
		snprintf(line, 512, "line %d", i);
		int error = BFFileWriterQueueLine(tools->fw, line);

		if (error) break;
	}

	BFFileWriterFlush(tools->fw);
}

//int test_writingfromdifferentthreads(void) {
BFTEST_UNIT_FUNC(test_writingfromdifferentthreads, 2<<4, {
	const int lines2write = 2 << 8;
	if (BFFileSystemPathExists(FILE_WRITER_FILE_PATH)) {
		remove(FILE_WRITER_FILE_PATH);
	}

	BFFileWriter fw;
	result = BFFileWriterCreate(&fw, FILE_WRITER_FILE_PATH);

	// Launch writing threads and wait for them to finish
	BFThreadAsyncID tid0 = 0;
	BFThreadAsyncID tid1 = 0;
	TestFileWriterThreadsTools tools;
	if (!result) {
		tools.fw = &fw;
		srand(time(0));
		tools.lines2write = lines2write;
		tid0 = BFThreadAsync(TestFileWriterThreads, (void *) &tools);
		tid1 = BFThreadAsync(TestFileWriterThreads, (void *) &tools);

		if (BFThreadAsyncError(tid0)) {
			printf("\ntid0 error %d\n", BFThreadAsyncError(tid0));
		} else if (BFThreadAsyncError(tid1)) {
			printf("\ntid1 error %d\n", BFThreadAsyncError(tid1));
		}
		fflush(stdout);

		while (BFThreadAsyncIsRunning(tid0) || BFThreadAsyncIsRunning(tid1)) { }

		BFThreadAsyncDestroy(tid0);
		BFThreadAsyncDestroy(tid1);
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

		if (i != (tools.lines2write * 2)) {
			printf("\n%d != %d\n", i, (tools.lines2write * 2));
			result = 1000;
		}
	}

	fclose(f);

	if (!result) {
		result = BFFileWriterClose(&fw);
	}
})

//int test_writingwithformat(void) {
BFTEST_UNIT_FUNC(test_writingwithformat, 2<<4, {
	if (BFFileSystemPathExists(FILE_WRITER_FILE_PATH)) {
		remove(FILE_WRITER_FILE_PATH);
	}

	BFFileWriter fw;
	result = BFFileWriterCreate(&fw, FILE_WRITER_FILE_PATH);

	// write test lines
	const int lines = 2 << 4;
	if (!result) {
		for (int i = 0; i < lines; i++) {
			result = BFFileWriterQueueFormatLine(&fw, "line %d", i);
			if (result) break;
		}

		if (!result)
			BFFileWriterFlush(&fw);
	}

	FILE * f = 0;
	if (!result) {
		f = fopen(FILE_WRITER_FILE_PATH, "r");
		if (!f) result = max + 1000;
	}

	// read line by line and compare
	if (!result) {
		size_t s = 512;
		char * line0 = 0, line1[s];
		int i = 0;
		while (!result && (getline(&line0, &s, f) != -1)) {
			snprintf(line1, s, "line %d\n", i);

			if (strcmp(line0, line1)) {
				printf("\n%s\n%s\n", line0, line1);
				result = max + 2000;
			}
			
			i++;
		}
		free(line0);
	}

	fclose(f);
	if (!result) {
		result = BFFileWriterClose(&fw);
	}
})

//int test_filewritingisappending() {
BFTEST_UNIT_FUNC(test_filewritingisappending, 2<<3, {
	if (BFFileSystemPathExists(FILE_WRITER_FILE_PATH)) {
		remove(FILE_WRITER_FILE_PATH);
	}

	// write test lines
	const int lines = 2 << 4;
	for (int i = 0; i < lines; i++) {
		char line[512];
		snprintf(line, 512, "line %d", i);

		BFFileWriter fw;
		result = BFFileWriterCreate(&fw, FILE_WRITER_FILE_PATH);

		if (!result)
			result = BFFileWriterQueueLine(&fw, line);

		if (!result)
			result = BFFileWriterClose(&fw);

		if (result) break;
	}

	FILE * f = 0;
	if (!result) {
		f = fopen(FILE_WRITER_FILE_PATH, "r");
		if (!f) result = max + 1000;
	}

	// read line by line and compare
	size_t s = 512;
	char * line0 = 0, line1[s];
	int i = 0;
	while (!result && (getline(&line0, &s, f) != -1)) {
		snprintf(line1, s, "line %d\n", i);

		if (strcmp(line0, line1)) {
			result = max + 1000;
		}
		
		i++;
	}
	
	free(line0);
	fclose(f);

	if (!result) {
		if (i != lines)
			result = 4;
	}
})

BFTEST_UNIT_FUNC(test_filetruncation, 2<<4, {
	if (BFFileSystemPathExists(FILE_WRITER_FILE_PATH)) {
		remove(FILE_WRITER_FILE_PATH);
	}
	
	BFFileWriter fw;
	result = BFFileWriterCreate(&fw, FILE_WRITER_FILE_PATH);

	// write test lines
	const int lines = 2 << 10;
	if (!result) {
		for (int i = 0; i < lines; i++) {
			char line[512];
			snprintf(line, 512, "line %d", i);

			if (!result)
				result = BFFileWriterQueueLine(&fw, line);

			if (result) break;
		}
	}

	// truncate
	if (!result) {
		result = BFFileWriterTruncate(&fw);
	}

	if (!result)
		result = BFFileWriterClose(&fw);

	FILE * f = 0;
	if (!result) {
		f = fopen(FILE_WRITER_FILE_PATH, "r");
		if (!f) result = max + 4000;
	}

	int size = 0;
	if (!result) {
		fseek(f, 0, SEEK_END);
		size = ftell(f);
		fseek(f, 0, SEEK_SET);

		if (size != 0) {
			result = size + 5000;
		}
	}

	fclose(f);
})

BFTEST_COVERAGE_FUNC(filewriter_tests, {
	BFThreadResetStartedCount();
	BFThreadResetStoppedCount();

	if (BFFileSystemPathExists(FILE_WRITER_FILE_PATH)) {
		remove(FILE_WRITER_FILE_PATH);
	}

	BFTEST_LAUNCH(test_creatingfilewriter);
	BFTEST_LAUNCH(test_writingwithfilewriter);
	BFTEST_LAUNCH(test_writingfromdifferentthreads);
	BFTEST_LAUNCH(test_writingwithformat);
	BFTEST_LAUNCH(test_filewritingisappending);
	BFTEST_LAUNCH(test_filetruncation);

	if (BFFileSystemPathExists(FILE_WRITER_FILE_PATH)) {
		remove(FILE_WRITER_FILE_PATH);
	}
})

#endif // FILE_WRITER_TESTS_H

