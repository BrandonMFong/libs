/**
 * author: brando
 * date: 2/5/24
 */

#include "filewriter.h"
#include <stdio.h>
#include <stdlib.h>
#include <lock.h>

typedef struct {
	FILE * file;
	BFLock qlock;
} _FileWriter;

int FileWriterCreate(FileWriter * filewriter, const char * filepath) {
	if (!filewriter || !filepath) return -1;

	_FileWriter * fw = malloc(sizeof(_FileWriter));
	if (!fw) return -1;

	fw->file = fopen(filepath, "w");
	if (!fw->file) return -1;

	int error = BFLockCreate(&fw->qlock);
	if (error) return error;

	*filewriter = (FileWriter *) fw;

	return 0;
}

int FileWriterQueueLine(FileWriter * filewriter, const char * line) {
	if (!filewriter || !line) return -2;
	_FileWriter * fw = *filewriter;

	BFLockLock(&fw->qlock);
	fprintf(fw->file, "%s\n", line);
	BFLockUnlock(&fw->qlock);

	return 0;
}

int FileWriterFlush(FileWriter * filewriter) {
	if (!filewriter) return -2;
	_FileWriter * fw = *filewriter;
	BFLockLock(&fw->qlock);
	fflush(fw->file);
	BFLockUnlock(&fw->qlock);
	return 0;
}

int FileWriterClose(FileWriter * filewriter) {
	if (!filewriter) return -3;
	_FileWriter * fw = *filewriter;

	fclose(fw->file);

	int error = BFLockDestroy(&fw->qlock);
	if (error) return error;

	free(fw);

	return 0;
}

