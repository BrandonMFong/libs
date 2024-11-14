/**
 * author: Brando
 * date: 10/20/23
 */

#ifndef THREAD_TESTS_H
#define THREAD_TESTS_H

#include "clib_tests.h"
#include "thread.h"
#include "lock.h"

void CreatingThreadSyncRun(void * in) {
	if (in) {
		sleep(2);
		bool * flag = (bool *) in;
		*flag = true;
	}
}

//int test_CreatingThreadSync(void) {
BFTEST_UNIT_FUNC(test_CreatingThreadSync, 1, {
	bool threadRan = false;
	result = BFThreadSync(CreatingThreadSyncRun, &threadRan);

	if (!result && !threadRan) result = 1;
})

void CreatingThreadAsyncRun(void * in) {}

//int test_CreatingThreadAsync(void) {
BFTEST_UNIT_FUNC(test_CreatingThreadAsync, 1, {
	BFThreadAsyncID id = BFThreadAsync(CreatingThreadAsyncRun, NULL);
	result = BFThreadAsyncError(id);

	BFThreadAsyncDestroy(id);
})

void WaitingOnThreadLockRun(void * in) {
	BFLock * lock = (BFLock *) in;
	sleep(5);
	int error = BFLockRelease(lock);
	if (error) printf("\n%d\n", error);
}

//int test_WaitingOnThreadLock(void) {
BFTEST_UNIT_FUNC(test_WaitingOnThreadLock, 1, {
	BFThreadAsyncID id = 0;
	BFLock lock;
	result = BFLockCreate(&lock);

	if (!result) {
		id = BFThreadAsync(WaitingOnThreadLockRun, &lock);
		result = BFThreadAsyncError(id);
	}

	if (!result) {
		result = BFLockWait(&lock);
	}

	if (!result) result = BFLockDestroy(&lock);

	BFThreadAsyncDestroy(id);
})

typedef struct {
	BFLock * lock;
	int * count;
	int toAdd;
} LockAndUnlockArgs;

void LockAndUnlock(void * in) {
	LockAndUnlockArgs * args = (LockAndUnlockArgs *) in;
	if (args && args->lock && args->count) {
		for (int i = 0; i < args->toAdd; i++) {
			BFLockLock(args->lock);
			(*args->count)++;
			BFLockUnlock(args->lock);
		}
	}
}

//int test_LockAndUnlock(void) {
BFTEST_UNIT_FUNC(test_LockAndUnlock, 1, {
	BFThreadAsyncID tid1, tid2;
	LockAndUnlockArgs args1, args2;
	int count = 0;
	BFLock lock;
	result = BFLockCreate(&lock);

	srand(time(NULL));
	
	args1.lock = &lock;
	args1.toAdd = rand() % (2 << 16);
	args1.count = &count;
	
	args2.lock = &lock;
	args2.toAdd = rand() % (2 << 16);
	args2.count = &count;

	if (!result) {
		tid1 = BFThreadAsync(LockAndUnlock, &args1);
		result = BFThreadAsyncError(tid1);
	}

	if (!result) {
		tid2 = BFThreadAsync(LockAndUnlock, &args2);
		result = BFThreadAsyncError(tid2);
	}

	if (!result) {
		while (1) {
			bool done = false;
			BFLockLock(&lock);
			if (count == (args1.toAdd + args2.toAdd)) done = true;
			BFLockUnlock(&lock);
			sleep(1);
			if (done) break;
		}
	}

	if (!result) {
		result = BFLockDestroy(&lock);
	}

	BFThreadAsyncDestroy(tid1);
	BFThreadAsyncDestroy(tid2);
})

void ReleasingAsyncRun(void * in) {
	if (in) {
		BFLock * lock = in;
		sleep(2);
		BFLockRelease(lock);
	}
}

//int test_ReleasingAsyncID(void) {
BFTEST_UNIT_FUNC(test_ReleasingAsyncID, 1, {
	BFLock lock;
	result = BFLockCreate(&lock);

	BFThreadAsyncID id;
	if (!result) {
		id = BFThreadAsync(ReleasingAsyncRun, &lock);
		result = BFThreadAsyncError(id);
	}

	if (!result) {
		BFThreadAsyncDestroy(id);
		BFLockWait(&lock);
	}

	BFLockDestroy(&lock);
})

void CancelingAsyncThreadRun(void * in) {
	const BFThreadAsyncID tid = BFThreadAsyncGetID();
	while (BFThreadAsyncIDIsValid(tid) && !BFThreadAsyncIsCanceled(tid)) {
		sleep(5);
	}
}

//int test_CancelingAsyncThread(void) {
BFTEST_UNIT_FUNC(test_CancelingAsyncThread, 1, {
	bool flag = false;

	BFThreadAsyncID id = 0;
	id = BFThreadAsync(CancelingAsyncThreadRun, &flag);
	result = BFThreadAsyncError(id);

	if (!result) {
		result = BFThreadAsyncCancel(id);
	}

	if (!result) {
		if (flag) result = 1;
	}
	
	BFThreadAsyncDestroy(id);
})

//int test_CancelingAsyncThreadThatHasAlreadyFinished() {
BFTEST_UNIT_FUNC(test_CancelingAsyncThreadThatHasAlreadyFinished, 1, {
	bool flag = false;

	BFThreadAsyncID id;
	if (!result) {
		id = BFThreadAsync(CancelingAsyncThreadRun, &flag);
		result = BFThreadAsyncError(id);
	}

	if (!result) {
		result = BFThreadAsyncCancel(id);
		while (BFThreadAsyncIsRunning(id)) {}
		sleep(1);
	}

	if (!result) {
		if (flag) result = 1;
	}
	
	BFThreadAsyncDestroy(id);
})

void TestThreadCountThread(void * in) { }

//int test_threadCount() {
BFTEST_UNIT_FUNC(test_threadCount, 1, {
	BFThreadResetStartedCount();
	BFThreadResetStoppedCount();
	int threadcount = 2 << 14;
	for (int i = 0; i < threadcount; i++) {
		result = BFThreadSync(TestThreadCountThread, 0);
		if (result) break;
	}

	if (!result) {
		if (BFThreadGetStartedCount() != threadcount) {
			result = 2;
		} else if (BFThreadGetStoppedCount() != threadcount) {
			result = 3;
		}
	}
})

void TestThreadWait(void * in) {
	const BFThreadAsyncID tid = BFThreadAsyncGetID();
	while (BFThreadAsyncIDIsValid(tid) && !BFThreadAsyncIsCanceled(tid)) {
		usleep(20000);
	}

	usleep(20000);
}

//int test_threadwait() {
BFTEST_UNIT_FUNC(test_threadwait, 1, {
	int max = 2 << 4;
	while (max) {
		BFThreadAsyncID tid = BFThreadAsync(TestThreadWait, 0);
		result = BFThreadAsyncError(tid);

		if (!result) {
			BFThreadAsyncCancel(tid);
			BFThreadAsyncWait(tid);

			if (BFThreadAsyncIsRunning(tid)) {
				result = 1;
			}
		}

		BFThreadAsyncDestroy(tid);
		max--;
	}
})

BFTEST_COVERAGE_FUNC(thread_tests) {
	BFTEST_COVERAGE_START;

	BFTEST_LAUNCH(test_CreatingThreadSync);
	BFTEST_LAUNCH(test_CreatingThreadAsync);
	BFTEST_LAUNCH(test_WaitingOnThreadLock);
	BFTEST_LAUNCH(test_LockAndUnlock);
	BFTEST_LAUNCH(test_ReleasingAsyncID);
	BFTEST_LAUNCH(test_CancelingAsyncThread);
	BFTEST_LAUNCH(test_CancelingAsyncThreadThatHasAlreadyFinished);
	BFTEST_LAUNCH(test_threadCount);
	BFTEST_LAUNCH(test_threadwait);

	BFTEST_COVERAGE_END;
}

#endif // THREAD_TESTS_H

