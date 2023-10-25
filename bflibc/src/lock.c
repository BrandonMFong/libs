/**
 * author: brando
 * date: 10/25/23
 */

#include "lock.h"
#include <stdlib.h>
#include <pthread.h>
#include "free.h"

typedef struct {
	pthread_mutex_t mutex;
	pthread_cond_t cond;
} _BFLock;

bool BFLockIsValid(BFLock * _lock) {
	if (_lock == NULL) return false;
	else {
		BFLock lock = *_lock;
		if (lock == NULL) return false;
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
		if (pthread_mutex_destroy(&l->mutex)) return 2;
		else if (pthread_cond_destroy(&l->cond)) return 3;
		else {
			BFFree(l);
		}
	}
	return 0;
}

int BFLockWait(BFLock * lock) {
	if (lock == 0) return 1;
	else {
		_BFLock * l = (_BFLock *) *lock;
		if (pthread_mutex_lock(&l->mutex)) return 2;
		else if (pthread_cond_wait(&l->cond, &l->mutex)) return 3;
		else if (pthread_mutex_unlock(&l->mutex)) return 4;
	}

	return 0;
}

int BFLockRelease(BFLock * lock) {
	if (lock == 0) return 1;
	else {
		_BFLock * l = (_BFLock *) *lock;
		if (pthread_mutex_lock(&l->mutex)) return 2;
		else if (pthread_cond_signal(&l->cond)) return 3;
		else if (pthread_mutex_unlock(&l->mutex)) return 4;
	}

	return 0;
}

int BFLockLock(BFLock * lock) {
	if (lock == 0) return 1;
	else {
		_BFLock * l = (_BFLock *) *lock;
		if (pthread_mutex_lock(&l->mutex)) return 2;
	}
	return 0;
}

int BFLockUnlock(BFLock * lock) {
	if (lock == 0) return 1;
	else {
		_BFLock * l = (_BFLock *) *lock;
		if (pthread_mutex_unlock(&l->mutex)) return 2;
	}
	return 0;
}

