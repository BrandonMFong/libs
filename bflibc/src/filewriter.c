/**
 * author: brando
 * date: 2/5/24
 */

#include "filewriter.h"
#include <stdio.h>
#include <stdlib.h>
#include "lock.h"
#include "thread.h"

typedef struct LineQueueItem {
	struct LineQueueItem * next;
	char * line;
} LineQueueItem;

typedef struct {
	LineQueueItem * top;
	size_t size;	
} LineQueue;

typedef struct {
	FILE * file;
	BFLock qlock;
	LineQueue q;
	BFThreadAsyncID tid;
} _FileWriter;

void FileWriterQueueThread(void * in) {
}

int FileWriterCreate(FileWriter * filewriter, const char * filepath) {
	if (!filewriter || !filepath) return -1;

	_FileWriter * fw = malloc(sizeof(_FileWriter));
	if (!fw) return -1;

	// open the file
	fw->file = fopen(filepath, "w");
	if (!fw->file) return -1;

	// init lock
	int error = BFLockCreate(&fw->qlock);
	if (error) return error;

	// init queue
	fw->q.size = 0;
	fw->q.top = NULL;

	// init thread
	fw->tid = BFThreadAsync(FileWriterQueueThread, (void *) fw);
	error = BFThreadAsyncIDError(fw->tid);
	if (error) return error;

	*filewriter = (FileWriter *) fw;

	return 0;
}

int FileWriterClose(FileWriter * filewriter) {
	if (!filewriter) return -3;
	_FileWriter * fw = *filewriter;

	// destroy thread
	BFThreadAsyncCancel(fw->tid);
	BFThreadAsyncIDDestroy(fw->tid);
	
	// release lock
	int error = BFLockDestroy(&fw->qlock);
	if (error) return error;

	// close file
	fclose(fw->file);

	free(fw);

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

