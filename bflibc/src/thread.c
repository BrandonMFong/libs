/**
 * author: brando
 * date: 10/20/23
 */

#define _GNU_SOURCE

#include "thread.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include "free.h"

#define _BFThreadTypeSync 1
#define _BFThreadTypeAsync -1
#define _BFThreadTypeAsyncDetached -2 // TODO: remove

/** start THREAD IDS **/

/// sync thread id
typedef void * _BFThreadSyncID;

/**
 * async id
 */
typedef struct {
	/**
	 * mutex for accessing our members
	 */
	pthread_mutex_t m;

	/**
	 * holds any error code that gets passed during
	 * creation of this object
	 */
	int error;

	/**
	 * main pthread object
	 */
	pthread_t p;
	
	/**
	 * holds attributes for `p`
	 */	
	pthread_attr_t attr;

	/**
	 * ||||||isCanceled|releaseQueued|isRunning|
	 *
	 * isRunning: true if callback has not returned
	 * releaseQueued: true if own has called destroy while we were running the callback
	 * isCanceled: true if owner called `BFThreadAsyncCancel`
	 */
	unsigned char flags;
} _BFThreadAsyncID;

#define FLAGS_GET(flags, bit) (flags & (1 << bit))
#define FLAGS_SET_ON(flags, bit) flags |= (1 << bit)
#define FLAGS_SET_OFF(flags, bit) flags &= ~(1 << bit)

#define IS_RUNNING_GET(flags) FLAGS_GET(flags, 0)
#define IS_RUNNING_SET_ON(flags) FLAGS_SET_ON(flags, 0)
#define IS_RUNNING_SET_OFF(flags) FLAGS_SET_OFF(flags, 0)

#define RELEASE_QUEUED_GET(flags) FLAGS_GET(flags, 1)
#define RELEASE_QUEUED_SET_ON(flags) FLAGS_SET_ON(flags, 1)
#define RELEASE_QUEUED_SET_OFF(flags) FLAGS_SET_OFF(flags, 1)

#define IS_CANCELED_GET(flags) FLAGS_GET(flags, 2)
#define IS_CANCELED_SET_ON(flags) FLAGS_SET_ON(flags, 2)
#define IS_CANCELED_SET_OFF(flags) FLAGS_SET_OFF(flags, 2)

/** end THREAD IDS **/
/** start THREAD ID TABLE **/

/**
 * pairs posix thread id with our BFThreadID
 */
typedef struct {
	pid_t posix_tid;
	char type; // sync = _BFThreadTypeSync, async = _BFThreadTypeAsync

	// we don't own this memory
	union {
		const _BFThreadSyncID * sync;
		const _BFThreadAsyncID * async;
	} id;
} _ThreadIDEntry;

/**
 * table of thread id entries
 */
typedef struct {
	pthread_mutex_t mut;
	_ThreadIDEntry ** entries;
	size_t size;
} _ThreadIDTable;

// global table that holds onto instantiated thread ids
_ThreadIDTable _tidtable = {
	.mut = PTHREAD_MUTEX_INITIALIZER,
	.entries = NULL,
	.size = 0
};

int _ThreadIDTablePushID(void * bftid, char type) {
	pthread_mutex_lock(&_tidtable.mut);
	int error = 0;
	if (bftid == NULL) {
		error = 50;
	}

	// create the entry object
	_ThreadIDEntry * ent = NULL;
	if (!error) {
		ent = (_ThreadIDEntry *) malloc(sizeof(_ThreadIDEntry));
		if (ent == NULL) {
			error = 51;
		} else {
			ent->posix_tid = gettid(); // save tid
			ent->type = type; // save type

			// save bfid
			if (type == _BFThreadTypeAsync) {
				ent->id.async = bftid;
			} else if (type == _BFThreadTypeSync) {
				ent->id.sync = bftid;
			} else {
				error = 52;
			}
		}
	}

	// store entry into table
	if (!error) {
		// set mem
		_tidtable.entries = realloc(
			_tidtable.entries,
			sizeof(_ThreadIDTable *) * ++_tidtable.size
		);

		_tidtable.entries[_tidtable.size - 1] = ent;
	}

	pthread_mutex_unlock(&_tidtable.mut);
	return error;
}

int _ThreadIDTablePopID() {
	pthread_mutex_lock(&_tidtable.mut);

	// get entry and organize table
	_ThreadIDEntry * ent = NULL;
	pid_t tid = gettid();
	size_t size = _tidtable.size;
	for (int i = 0; i < size; i++) {
		// if we find the entry
		if (_tidtable.entries[i] != NULL) {
			if (_tidtable.entries[i]->posix_tid == tid) {
				ent = _tidtable.entries[i]; // save entry
				size--; // decrease threshold
			}
		}

		// shift elements if we found entry
		if (ent != NULL) {
			if (i < size)
				_tidtable.entries[i] = _tidtable.entries[i + 1];
		}
	}

	// free memory
	int error = 0;
	if (ent == NULL) {
		error = 61;
	} else {
		free(ent);
	}

	pthread_mutex_unlock(&_tidtable.mut);
	return error;
}

const void * _ThreadIDTableGetID() {
	const void * result = NULL;
	pthread_mutex_lock(&_tidtable.mut);
	pid_t tid = gettid();
	for (int i = 0; i < _tidtable.size; i++) {
		if (_tidtable.entries[i]->posix_tid == tid) {
			_ThreadIDEntry * ent = _tidtable.entries[i];
			if (ent->type == _BFThreadTypeAsync) {
				result = ent->id.async;
			} else if (ent->type == _BFThreadTypeSync) {
				result = ent->id.sync;
			}
		}
	}
	pthread_mutex_unlock(&_tidtable.mut);
	return result;
}

/** end THREAD ID TABLE **/

typedef struct {
	/// the function that will run on its dedicated thread
	void (* callback)(void *);
	void * args;
	char type; // sync = _BFThreadTypeSync, async = _BFThreadTypeAsync
	union {
		_BFThreadSyncID * sync;
		_BFThreadAsyncID * async;
	} id;
} _BFThreadRoutineParams;

/** start THREAD COUNTERS **/

static pthread_mutex_t _threadsStartedMut = PTHREAD_MUTEX_INITIALIZER;
int _threadsStarted = 0;
static pthread_mutex_t _threadsStoppedMut = PTHREAD_MUTEX_INITIALIZER;
int _threadsStopped = 0;

int BFThreadGetStartedCount() {
	int result = 0;
	pthread_mutex_lock(&_threadsStartedMut);
	result = _threadsStarted;
	pthread_mutex_unlock(&_threadsStartedMut);
	return result;
}

void _BFThreadIncrementStartedCount() {
	pthread_mutex_lock(&_threadsStartedMut);
	_threadsStarted++;
	pthread_mutex_unlock(&_threadsStartedMut);
}

void BFThreadResetStartedCount() {
	pthread_mutex_lock(&_threadsStartedMut);
	_threadsStarted = 0;
	pthread_mutex_unlock(&_threadsStartedMut);
}

int BFThreadGetStoppedCount() {
	int result = 0;
	pthread_mutex_lock(&_threadsStoppedMut);
	result = _threadsStopped;
	pthread_mutex_unlock(&_threadsStoppedMut);
	return result;
}

void _BFThreadIncrementStoppedCount() {
	pthread_mutex_lock(&_threadsStoppedMut);
	_threadsStopped++;
	pthread_mutex_unlock(&_threadsStoppedMut);
}

void BFThreadResetStoppedCount() {
	pthread_mutex_lock(&_threadsStoppedMut);
	_threadsStopped = 0;
	pthread_mutex_unlock(&_threadsStoppedMut);
}

/** start THREAD COUNTERS **/

void * _BFThreadStartRoutine(void * _params) {
	_BFThreadIncrementStartedCount();
	if (_params) {
		_BFThreadRoutineParams * params = _params;

		// allow this thread to be canceled
		if (params->type == _BFThreadTypeAsync) {
			pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
		}

		// push the id to the table so it
		// can be called by caller anytime
		if (params->type == _BFThreadTypeAsync) {
			_ThreadIDTablePushID(params->id.async, params->type);
		} else if (params->type == _BFThreadTypeSync) {
			_ThreadIDTablePushID(params->id.sync, params->type);
		}

		// run the caller defined function on current
		// thread
		if (params->callback)
			params->callback(params->args);

		// pop id from the thread table
		_ThreadIDTablePopID();

		// memory management
		if (params->type == _BFThreadTypeAsync) {
			pthread_mutex_lock(&params->id.async->m);

			IS_RUNNING_SET_OFF(params->id.async->flags);

			// See if user called BFThreadAsyncIDDestroy
			bool doRelease = RELEASE_QUEUED_GET(params->id.async->flags);

			pthread_mutex_unlock(&params->id.async->m);

			if (doRelease)
				BFFree(params->id.async);
		}

		// We own memory
		BFFree(_params);
	}
	
	_BFThreadIncrementStoppedCount();

	return NULL;
}

const BFThreadAsyncID BFThreadAsyncGetID() {
	return (const BFThreadAsyncID) _ThreadIDTableGetID();
}

void BFThreadAsyncDestroy(BFThreadAsyncID in) {
	if (in) {
		_BFThreadAsyncID * id = (_BFThreadAsyncID *) in;
		pthread_mutex_lock(&id->m);

		// free the id if the thread is not running
		//
		// if the thread is running then the thread
		// will be released by _BFThreadStartRoutine
		bool doRelease = false;
		//if (id->isRunning) {
		if (IS_RUNNING_GET(id->flags)) {
			// This flag will get checked when the thread terminates
			RELEASE_QUEUED_SET_ON(id->flags);
		} else {
			doRelease = true;
		}

		pthread_mutex_unlock(&id->m);

		if (doRelease) {
			pthread_mutex_destroy(&id->m);
			BFFree(id);
		}
	}
}

BFThreadAsyncID BFThreadAsync(
	void (* callback)(void *),
	void * args
) {
	// result can get released by caller or automatically by
	// async thread
	_BFThreadAsyncID * result = malloc(sizeof(_BFThreadAsyncID));
	int error = result == NULL ? 1 : 0;

	if (!error) {
		if (pthread_mutex_init(&result->m, NULL)) error = 1;
		else {
			result->error = 0;
			IS_RUNNING_SET_OFF(result->flags);
			RELEASE_QUEUED_SET_OFF(result->flags);
			IS_CANCELED_SET_OFF(result->flags);
		}
	}

	_BFThreadRoutineParams * params = NULL;
	if (!error) {
		params = malloc(sizeof(_BFThreadRoutineParams)); // _BFThreadStartRoutine will free memory
		if (params == NULL) {
			error = 1;
		} else {
			params->callback = callback;
			params->args = args;
			params->type = _BFThreadTypeAsync;
			params->id.async = result;
		}
	}

	// Set attribute
	if (!error)
		error = pthread_attr_init(&result->attr);

	if (!error)
		error = pthread_attr_setdetachstate(&result->attr, PTHREAD_CREATE_DETACHED);


	if (!error) {
		IS_RUNNING_SET_ON(result->flags);
		result->error = error;
		pthread_create(&result->p, &result->attr, _BFThreadStartRoutine, (void *) params);
	}

	return result;
}

bool BFThreadAsyncIDIsValid(BFThreadAsyncID id) {
	if (id) return true;
	return false;
}

int BFThreadAsyncError(BFThreadAsyncID id) {
	if (id) return ((_BFThreadAsyncID *) id)->error;
	return 0;
}

bool BFThreadAsyncIsRunning(BFThreadAsyncID in) {
	bool result = false;

	if (in) {
		_BFThreadAsyncID * id = (_BFThreadAsyncID *) in;
		pthread_mutex_lock(&id->m);
		result = IS_RUNNING_GET(id->flags);
		pthread_mutex_unlock(&id->m);
	}

	return result;
}

int BFThreadAsyncCancel(BFThreadAsyncID in) {
	if (!in) return 1;
	else {
		_BFThreadAsyncID * id = (_BFThreadAsyncID *) in;
		pthread_mutex_lock(&id->m);
		IS_CANCELED_SET_ON(id->flags);
		pthread_mutex_unlock(&id->m);
		return 0;
	}
}

bool BFThreadAsyncIsCanceled(BFThreadAsyncID in) {
	if (!in) return false;
	else {
		_BFThreadAsyncID * id = (_BFThreadAsyncID *) in;
		pthread_mutex_lock(&id->m);
		bool result = IS_CANCELED_GET(id->flags);
		pthread_mutex_unlock(&id->m);
		return result;
	}
}

int BFThreadSync(void (* callback)(void *), void * args) {
	int error = 0;
	
	// _BFThreadStartRoutine will free memory
	_BFThreadRoutineParams * params = malloc(sizeof(_BFThreadRoutineParams));

	error = params == NULL ? 1 : 0;
	if (!error) {
		params->callback = callback;
		params->args = args;
		params->type = _BFThreadTypeSync;
		params->id.sync = 0;
	}
	
	pthread_attr_t attr;
	if (!error)
		error = pthread_attr_init(&attr);

	if (!error)
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	pthread_t p;
	if (!error)
		error = pthread_create(&p, &attr, _BFThreadStartRoutine, (void *) params);

	if (!error)
		error = pthread_join(p, NULL);

	return error;
}

int BFThreadAsyncDetach(void (* callback)(void *), void * args) {
	return 0;
}

