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
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>

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
	item->next = NULL;
	item->line = BFStringCopyString(line);
	if (!item->line) return -32;

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
	if (item) {
		free(item->line);
		free(item);
	}
	BFLockUnlock(&q->lock);

	return 0;
}

const char * _LineQueueGetTopLine(_LineQueue * q) {
	if (!q) return 0;
	const char * result = 0;
	BFLockLock(&q->lock);
	if (q->size > 0) {
		result = q->first->line;
	}
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

	// the path `file` is writing to
	char path[PATH_MAX];

	_LineQueue q;
	BFThreadAsyncID tid;
	
	// lock access for this object
	BFLock lock;
} _FileWriter;

/**
 * dedicated thread that will write into file
 */
void _FileWriterQueueThread(void * in) {
	_FileWriter * fw = in;
	if (!fw) return;
	while (fw && !BFThreadAsyncIsCanceled(fw->tid)) {
		if (_LineQueueGetSize(&fw->q) == 0) {
			usleep(500);
		} else {
			const char * line = _LineQueueGetTopLine(&fw->q);
		
			BFLockLock(&fw->lock);
			fprintf(fw->file, "%s\n", line);
			BFLockUnlock(&fw->lock);
	
			_LineQueuePop(&fw->q);
		}
	}
}

int BFFileWriterCreate(BFFileWriter * filewriter, const char * filepath) {
	if (!filewriter || !filepath) return -1;

	_FileWriter * fw = malloc(sizeof(_FileWriter));
	if (!fw) return -1;

	// open the file
	fw->file = fopen(filepath, "a");
	if (!fw->file) return -1;

	// save path
	strncpy(fw->path, filepath, PATH_MAX);

	// init queue lock
	int error = BFLockCreate(&fw->q.lock);
	if (error) return error;

	// init our lock
	error = BFLockCreate(&fw->lock);
	if (error) return error;

	// init queue
	fw->q.size = 0;
	fw->q.first = NULL;
	fw->q.last = NULL;

	// init thread
	fw->tid = BFThreadAsync(_FileWriterQueueThread, (void *) fw);
	error = BFThreadAsyncError(fw->tid);
	if (error) return error;

	*filewriter = (BFFileWriter *) fw;

	return 0;
}

int BFFileWriterClose(BFFileWriter * filewriter) {
	if (!filewriter) return -3;
	_FileWriter * fw = *filewriter;
	if (!fw) return -3;

	// make sure all lines are written
	int error = BFFileWriterFlush(filewriter);

	if (!error) {
		// tell workloop to stop looping
		BFThreadAsyncCancel(fw->tid);

		// wait for the thread to exit
		BFThreadAsyncWait(fw->tid);

		// destroy thread
		BFThreadAsyncDestroy(fw->tid);
	}
	
	// release q lock
	if (!error)
		error = BFLockDestroy(&fw->q.lock);
	
	// release our lock
	if (!error)
		error = BFLockDestroy(&fw->lock);
	
	// close file & free mem
	fclose(fw->file);
	free(fw);

	return 0;
}

int BFFileWriterTruncate(BFFileWriter * filewriter) {
	if (!filewriter) return -2;
	_FileWriter * fw = *filewriter;
	if (!fw) return -3;

	// make sure all lines are written
	//
	// is this efficient?
	int error = BFFileWriterFlush(filewriter);

	if (!error) {
		BFLockLock(&fw->lock);
		FILE * f = fopen(fw->path, "w");
		fclose(f);
		BFLockUnlock(&fw->lock);
	}

	return 0;
}

int BFFileWriterQueueLine(BFFileWriter * filewriter, const char * line) {
	if (!filewriter || !line) return -2;
	_FileWriter * fw = *filewriter;
	if (!fw) return -3;
	return _LineQueuePush(&fw->q, line);
}

int BFFileWriterFlush(BFFileWriter * filewriter) {
	if (!filewriter) return -4;
	_FileWriter * fw = *filewriter;
	if (!fw) return -3;

	while (_LineQueueGetSize(&fw->q)) { }

	BFLockLock(&fw->lock);
	fflush(fw->file);
	BFLockUnlock(&fw->lock);
	return 0;
}

int BFFileWriterQueueFormatLine(BFFileWriter * filewriter, const char * format, ...) {
	if (!filewriter || !format) return -2;
	_FileWriter * fw = *filewriter;
	if (!fw) return -3;

	va_list valist;
	va_start(valist, format);

	char * line = BFStringCreateFormatArgListString(format, valist);
	int result = _LineQueuePush(&fw->q, line);

	free(line);

	va_end(valist);

	return result;
}

