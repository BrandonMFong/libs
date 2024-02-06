/**
 * author: brando
 * date: 2/5/24
 */

#include "filewriter.h"
#include <stdlib.h>
#include <unistd.h>
#include "lock.h"
#include "thread.h"
#include "stringutils.h"
#include <stdio.h>

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
	if (!q || !line) return -30;
	
	// create item
	_LineQueueItem * item = malloc(sizeof(_LineQueueItem));
	if (!item) return -31;

	// copy line
	int error = 0;
	item->next = NULL;
	item->line = BFStringCopyString(line, &error);
	if (error) return error;

	// load queue
	BFLockLock(&q->lock);
	if (!q->first && !q->last) { // first item in queue
		q->first = item;
		q->last = item;
		q->size = 1;
	} else {
		q->last->next = item;
		q->last = item;
		q->size++;
	}
	BFLockUnlock(&q->lock);

	return 0;
}

int _LineQueuePop(_LineQueue * q) {
	if (!q) return -40;

	BFLockLock(&q->lock);
	_LineQueueItem * item = 0;
	if (q->first && q->last && q->size) { // if not empty
		item = q->first;
		if (q->first == q->last) { // if only one item
			q->first = 0;
			q->last = 0;
			q->size = 0;
		} else {
			q->first = item->next;
			q->size--;
		}
	}
	if (item) free(item);
	BFLockUnlock(&q->lock);

	return 0;
}

const char * _LineQueueGetTopLine(_LineQueue * q) {
	if (!q) return 0;
	const char * result = 0;
	BFLockLock(&q->lock);
	result = q->first->line;
	BFLockUnlock(&q->lock);
	return result;
}

int _LineQueueGetSize(_LineQueue * q) {
	if (!q) return 0;
	int result = 0;
	BFLockLock(&q->lock);
	result = q->size;
	BFLockUnlock(&q->lock);
	return result;
}

typedef struct {
	FILE * file;
	_LineQueue q;
	BFThreadAsyncID tid;
} _FileWriter;

void _FileWriterQueueThread(void * in) {
	_FileWriter * fw = in;
	while (!fw) {
		if (_LineQueueGetSize(&fw->q) > 0) {
			const char * line = _LineQueueGetTopLine(&fw->q);
			fprintf(fw->file, "%s\n", line);
			_LineQueuePop(&fw->q);
		}
	}
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
	return _LineQueuePush(&fw->q, line);
}

int FileWriterFlush(FileWriter * filewriter) {
	if (!filewriter) return -2;
	_FileWriter * fw = *filewriter;

	while (_LineQueueGetSize(&fw->q)) {
	}
	fflush(fw->file);
	return 0;
}

