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
	int error;
	pthread_t p;
	bool isRunning;
	pthread_mutex_t m;
	pthread_attr_t attr;
	bool ownerQueuedRelease;
} _BFThreadAsyncID;

typedef struct {
	void (* callback)(void *);
	void * args;
	char type; // sync = _BFThreadTypeSync, async = _BFThreadTypeAsync
	union {
		_BFThreadSyncID * sync;
		_BFThreadAsyncID * async;
	} id;
} _BFThreadRoutineParams;

void * _BFThreadStartRoutine(void * _params) {
	if (_params) {
		_BFThreadRoutineParams * params = _params;
		
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
			params->id.async->isRunning = false;

			// See if user called BFThreadAsyncIDDestroy
			doRelease = params->id.async->ownerQueuedRelease;

			pthread_mutex_unlock(&params->id.async->m);

			if (doRelease)
				BFFree(params->id.async);
		}

		// We own memory
		BFFree(_params);
	}
	return NULL;
}

void BFThreadAsyncIDDestroy(BFThreadAsyncID in) {
	if (in) {
		_BFThreadAsyncID * id = (_BFThreadAsyncID *) in;
		pthread_mutex_lock(&id->m);

		// free the id if the thread is not running
		//
		// if the thread is running then the thread
		// will be released by _BFThreadStartRoutine
		bool doRelease = false;
		if (id->isRunning) {
			// This flag will get checked when the thread terminates
			id->ownerQueuedRelease = true;
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
			result->isRunning = false;
			result->ownerQueuedRelease = false;
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
	//pthread_attr_t attr;
	if (!error)
		error = pthread_attr_init(&result->attr);

	if (!error)
		error = pthread_attr_setdetachstate(&result->attr, PTHREAD_CREATE_JOINABLE);

	if (!error) {
		result->isRunning = true;
		result->error = error;
		pthread_create(&result->p, &result->attr, _BFThreadStartRoutine, (void *) params);
	}

	return result;
}

int BFThreadAsyncIDError(BFThreadAsyncID id) {
	if (id) return ((_BFThreadAsyncID *) id)->error;
	return 0;
}

bool BFThreadAsyncIDIsRunning(BFThreadAsyncID in) {
	bool result = false;

	if (in) {
		_BFThreadAsyncID * id = (_BFThreadAsyncID *) in;
		pthread_mutex_lock(&id->m);
		result = id->isRunning;
		pthread_mutex_unlock(&id->m);
	}

	return result;
}

int BFThreadAsyncCancel(BFThreadAsyncID in) {
	if (!in) return 1;
	else {
		_BFThreadAsyncID * id = (_BFThreadAsyncID *) in;

		if (id->isRunning) {
			id->isRunning = false;
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

