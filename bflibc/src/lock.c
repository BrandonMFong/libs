/**
 * author: brando
 * date: 10/25/23
 */

#include "lock.h"
#include <stdlib.h>
#include <pthread.h>
#include "free.h"
#include "bftime.h"
#include <errno.h>

typedef struct {
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	unsigned char flags;
} _BFLock;

#define FLAGS_GET(flags, bit) (flags & (1 << bit))
#define FLAGS_SET_ON(flags, bit) flags |= (1 << bit)
#define FLAGS_SET_OFF(flags, bit) flags &= ~(1 << bit)

#define IS_WAITING_GET(flags) FLAGS_GET(flags, 0)
#define IS_WAITING_SET_ON(flags) FLAGS_SET_ON(flags, 0)
#define IS_WAITING_SET_OFF(flags) FLAGS_SET_OFF(flags, 0)

bool BFLockIsValid(BFLock * _lock) {
	if (_lock == NULL) return false;
	else {
		_BFLock * l = (_BFLock *) *_lock;
		if (l == NULL) return false;
	}
	return true;
}

int BFLockCreate(BFLock * lock) {
	int error = 0;
	_BFLock * l = (_BFLock *) malloc(sizeof(_BFLock));
	if (l == 0) error = 1;
	else if (lock == NULL) error = 1;
	else if (pthread_mutex_init(&l->mutex, NULL)) error = 1;
	else if (pthread_cond_init(&l->cond, NULL)) error = 1;
	else {
		*lock = (BFLock) l;
	}

	return error;
}

int BFLockDestroy(BFLock * lock) {
	if (lock == 0) return 1;
	else {
		_BFLock * l = (_BFLock *) *lock;

		int error = pthread_mutex_destroy(&l->mutex);
		if (error) return error;
	
		error = pthread_cond_destroy(&l->cond);
		if (error) return error;
		
		BFFree(l);
	}
	return 0;
}

int BFLockWait(BFLock * lock) {
	if (lock == 0) return 1;
	else {
		_BFLock * l = (_BFLock *) *lock;
		if (pthread_mutex_lock(&l->mutex)) return 202;
		
		IS_WAITING_SET_ON(l->flags);

		if (pthread_cond_wait(&l->cond, &l->mutex)) return 3;
		if (pthread_mutex_unlock(&l->mutex)) return 4;
	}

	return 0;
}

int BFLockTimedWait(BFLock * lock, BFTime t) {
	struct timespec ts;
	_BFLock * l = 0;
	if (lock == 0) return 1;
	else {
		l = (_BFLock *) *lock;
		
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += (time_t) t;
		ts.tv_nsec += BFTimeGetNS(t);

		if (pthread_mutex_lock(&l->mutex)) return 203;
	}
	
	IS_WAITING_SET_ON(l->flags);

	// Check if wait timedout
	int result = 0;
	int err = pthread_cond_timedwait(&l->cond, &l->mutex, &ts);
	if ((err == ETIMEDOUT) || (err == EAGAIN)) {
		result = kBFLockTimedWaitCodeTimedOut;
	}
	
	if (pthread_mutex_unlock(&l->mutex)) result = 4;

	return result;
}

int BFLockRelease(BFLock * lock) {
	if (lock == 0) return 1;
	else {
		_BFLock * l = (_BFLock *) *lock;
		if (pthread_mutex_lock(&l->mutex)) return 204;
		IS_WAITING_SET_OFF(l->flags);
		if (pthread_cond_signal(&l->cond)) return 3;
		if (pthread_mutex_unlock(&l->mutex)) return 4;
	}

	return 0;
}

int BFLockLock(BFLock * lock) {
	if (lock == 0) return 1;
	else {
		_BFLock * l = (_BFLock *) *lock;
		if (pthread_mutex_lock(&l->mutex)) {
			return 205;
		}
	}
	return 0;
}

int BFLockUnlock(BFLock * lock) {
	if (lock == 0) return 1;
	else {
		_BFLock * l = (_BFLock *) *lock;
		if (pthread_mutex_unlock(&l->mutex)) {
			return 206;
		}
	}
	return 0;
}

bool BFLockIsWaiting(BFLock * lock) {
	if (lock == 0) return false;
	else {
		_BFLock * l = (_BFLock *) *lock;
		pthread_mutex_lock(&l->mutex);
		bool res = IS_WAITING_GET(l->flags);
		pthread_mutex_unlock(&l->mutex);
		return res;
	}
}

