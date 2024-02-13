/**
 * author: brando
 * date: 10/20/23
 */

#include "thread.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "free.h"

#define _BFThreadTypeSync 1
#define _BFThreadTypeAsync -1
#define _BFThreadTypeAsyncDetached -2

typedef void * _BFThreadSyncID;

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
	 * |||||||releaseQueued|isRunning|
	 *
	 * isRunning: true if callback has not returned
	 * releaseQueued: true if own has called destroy while we were running the callback
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

void * _BFThreadStartRoutine(void * _params) {
	_BFThreadIncrementStartedCount();
	if (_params) {
		_BFThreadRoutineParams * params = _params;
		
		// allow this thread to be canceled
		if (params->type == _BFThreadTypeAsync) {
			pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
			pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
		}

		// run the caller defined function on current
		// thread
		if (params->callback)
			params->callback(params->args);

		if (params->type == _BFThreadTypeAsync) {
			bool doRelease = false;
			
			pthread_mutex_lock(&params->id.async->m);
			IS_RUNNING_SET_OFF(params->id.async->flags);

			// See if user called BFThreadAsyncIDDestroy
			doRelease = RELEASE_QUEUED_GET(params->id.async->flags);

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

		if (BFThreadAsyncIsRunning(id)) {
			pthread_mutex_lock(&id->m);
			IS_RUNNING_SET_OFF(id->flags);
			pthread_mutex_unlock(&id->m);

			return pthread_cancel(id->p);
		} else {
			return 0;
		}
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
	int error = 0;
	
	// _BFThreadStartRoutine will free memory
	_BFThreadRoutineParams * params = malloc(sizeof(_BFThreadRoutineParams));

	error = params == NULL ? 1 : 0;
	if (!error) {
		params->callback = callback;
		params->args = args;
		params->type = _BFThreadTypeAsyncDetached;
	}
	
	pthread_attr_t attr;
	error = pthread_attr_init(&attr);
	if (!error)
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	
	pthread_t p;
	if (!error)
		error = pthread_create(&p, &attr, _BFThreadStartRoutine, (void *) params);

	return error;
}

