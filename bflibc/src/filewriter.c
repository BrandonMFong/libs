/**
 * author: brando
 * date: 2/5/24
 */

#include "filewriter.h"
#include <stdio.h>
#include <stdlib.h>
#include "lock.h"
#include "thread.h"
#include "stringutils.h"

typedef struct _LineQueueItem {
	struct _LineQueueItem * next;
	char * line;
} _LineQueueItem;

typedef struct {
	_LineQueueItem * first;
	_LineQueueItem * last;
	size_t size;	
	BFLock lock;
} _LineQueue;

int _LineQueuePush(_LineQueue * q, const char * line) {
	if (!q || !line) return -2;
	
	// create item
	_LineQueueItem * item = malloc(sizeof(_LineQueueItem));
	if (!item) return -1;

	// copy line
	int error = 0;
	item->line = BFStringCopyString(line, &error);
	if (error) return error;

	// load queue
	if (!q->first && !q->last) { // first item in queue
		q->first = item;
		q->last = item;
		q->size = 1;
	} else {
		q->last->next = item;
		q->last = item;
		q->size++;
	}

	return 0;
}

int _LineQueuePop(_LineQueue * q) {
	return 0;
}

typedef struct {
	FILE * file;
	_LineQueue q;
	BFThreadAsyncID tid;
} _FileWriter;

void _FileWriterQueueThread(void * in) {
}

int FileWriterCreate(FileWriter * filewriter, const char * filepath) {
	if (!filewriter || !filepath) return -1;

	_FileWriter * fw = malloc(sizeof(_FileWriter));
	if (!fw) return -1;

	// open the file
	fw->file = fopen(filepath, "w");
	if (!fw->file) return -1;

	// init lock
	int error = BFLockCreate(&fw->q.lock);
	if (error) return error;

	// init queue
	fw->q.size = 0;
	fw->q.first = NULL;
	fw->q.last = NULL;

	// init thread
	fw->tid = BFThreadAsync(_FileWriterQueueThread, (void *) fw);
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
	int error = BFLockDestroy(&fw->q.lock);
	if (error) return error;

	// close file
	fclose(fw->file);

	free(fw);

	return 0;
}

int FileWriterQueueLine(FileWriter * filewriter, const char * line) {
	if (!filewriter || !line) return -2;
	_FileWriter * fw = *filewriter;

	BFLockLock(&fw->q.lock);
	fprintf(fw->file, "%s\n", line);
	BFLockUnlock(&fw->q.lock);

	return 0;
}

int FileWriterFlush(FileWriter * filewriter) {
	if (!filewriter) return -2;
	_FileWriter * fw = *filewriter;
	BFLockLock(&fw->q.lock);
	fflush(fw->file);
	BFLockUnlock(&fw->q.lock);
	return 0;
}

