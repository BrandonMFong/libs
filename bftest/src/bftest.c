/**
 * author: brando
 * date: 11/13/24
 */

#include "bftest.h"
#include <stdarg.h>
#include <string.h>

typedef struct _TestLogQueueEntry {
	struct _TestLogQueueEntry * next;
	char * msg;
	char filename[256];
	int line;
	_BFTestLogType logtype;
	char expression[256];
} _TestLogQueueEntry;

char * _Arg2String(const char * format, va_list valist) {
	va_list args0, args1;
	va_copy(args0, valist);
	va_copy(args1, valist);

	// get size
	int size = vsnprintf(0, 0, format, args0);

	// create buffer
	char * result = malloc(sizeof(char) * (size + 1));
	if (!result) return NULL;

	if (vsnprintf(result, size + 1, format, args1) < 0) {
		free(result);
		return NULL;
	}

	return result;
}

_TestLogQueueEntry * _TestLogQueueEntryAlloc(
	const char * filename,
	int line,
	_BFTestLogType logtype,
	const char * expression,
	const char * format,
	va_list valist
) {
	if (!format) return NULL;

	_TestLogQueueEntry * res = (_TestLogQueueEntry *) malloc(
		sizeof(_TestLogQueueEntry)
	);
	if (!res) return NULL;

	res->next = NULL;
	res->msg = _Arg2String(format, valist);
	strcpy(res->filename, filename);
	res->line = line;
	res->logtype = logtype;
	strcpy(res->expression, expression);
	return res;
}

void _TestLogQueueEntryRelease(_TestLogQueueEntry * ent) {
	free(ent->msg);
	free(ent);
}

typedef struct _TestLogQueue {
	_TestLogQueueEntry * head;
	_TestLogQueueEntry * tail;
	size_t size;
} _TestLogQueue;

// https://www.geeksforgeeks.org/queue-in-c/
_TestLogQueue q = {0};

void _TestLogQueueEnqueue(
	const char * filename,
	int line,
	_BFTestLogType logtype,
	const char * expression,
	const char * format,
	va_list valist
) {
	_TestLogQueueEntry * ent = _TestLogQueueEntryAlloc(
		filename,
		line,
		logtype,
		expression,
		format,
		valist
	);

	if (!q.head) {
		q.head = q.tail = ent;
	} else {
		_TestLogQueueEntry * curr = q.head;
		while (!curr->next) { curr = curr->next; }
		q.tail = curr->next = ent;
	}
	q.size++;
}

void _TestLogQueueDequeue() {
	if (q.head) {
		_TestLogQueueEntry * ent = q.head;
		q.head = q.head->next;
		_TestLogQueueEntryRelease(ent);
		q.size--;
	}
}

_TestLogQueueEntry * _TestLogQueuePeek() {
	return q.head;
}

void _BFTestLogPush(
	const char * filename,
	int line,
	_BFTestLogType logtype,
	const char * expression,
	const char * format,
	...
) {
	//if (strlen(format) == 0) return;
	va_list valist;
	va_start(valist, format);
	_TestLogQueueEnqueue(
		filename, line, logtype,
		expression,
		format, valist
	);
	va_end(valist);
}

void _BFTestLogFlush() {
	_TestLogQueueEntry * curr = _TestLogQueuePeek();
	while (curr) {
		switch (curr->logtype) {
		case _kBFTestLogTypeAssertFailure:
		default:
			printf(" * %s:%d (expr: %s)%s%s\n",
				curr->filename,
				curr->line,
				curr->expression,
				strlen(curr->msg) > 0 ? " - " : "",
				curr->msg
			);
		}
		_TestLogQueueDequeue();
		curr = _TestLogQueuePeek();
	}
}

