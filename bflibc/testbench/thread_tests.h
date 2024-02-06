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

int test_CreatingThreadSync(void) {
	UNIT_TEST_START;
	int result = 0;

	bool threadRan = false;
	result = BFThreadSync(CreatingThreadSyncRun, &threadRan);

	if (!result && !threadRan) result = 1;

	UNIT_TEST_END(!result, result);
	return result;
}

void CreatingThreadAsyncRun(void * in) {}

int test_CreatingThreadAsync(void) {
	UNIT_TEST_START;
	int result = 0;

	BFThreadAsyncID id = BFThreadAsync(CreatingThreadAsyncRun, NULL);
	result = BFThreadAsyncIDError(id);

	BFThreadAsyncIDDestroy(id);

	UNIT_TEST_END(!result, result);
	return result;
}

void WaitingOnThreadLockRun(void * in) {
	BFLock * lock = (BFLock *) in;
	sleep(5);
	int error = BFLockRelease(lock);
	if (error) printf("\n%d\n", error);
}

int test_WaitingOnThreadLock(void) {
	UNIT_TEST_START;
	int result = 0;
	BFThreadAsyncID id;
	BFLock lock;
	result = BFLockCreate(&lock);

	if (!result) {
		id = BFThreadAsync(WaitingOnThreadLockRun, &lock);
		result = BFThreadAsyncIDError(id);
	}

	if (!result) {
		result = BFLockWait(&lock);
	}

	if (!result) result = BFLockDestroy(&lock);

	BFThreadAsyncIDDestroy(id);

	UNIT_TEST_END(!result, result);
	return result;
}

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

int test_LockAndUnlock(void) {
	UNIT_TEST_START;
	int result = 0;
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
		result = BFThreadAsyncIDError(tid1);
	}

	if (!result) {
		tid2 = BFThreadAsync(LockAndUnlock, &args2);
		result = BFThreadAsyncIDError(tid2);
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

	BFThreadAsyncIDDestroy(tid1);
	BFThreadAsyncIDDestroy(tid2);

	UNIT_TEST_END(!result, result);
	return result;
}

void ReleasingAsyncIDRun(void * in) {
	if (in) {
		BFLock * lock = in;
		sleep(2);
		BFLockRelease(lock);
	}
}

int test_ReleasingAsyncID(void) {
	UNIT_TEST_START;
	int result = 0;
	BFLock lock;
	result = BFLockCreate(&lock);

	BFThreadAsyncID id;
	if (!result) {
		id = BFThreadAsync(ReleasingAsyncIDRun, &lock);
		result = BFThreadAsyncIDError(id);
	}

	if (!result) {
		BFThreadAsyncIDDestroy(id);
		BFLockWait(&lock);
	}

	BFLockDestroy(&lock);

	UNIT_TEST_END(!result, result);
	return result;
}

void AsyncDetachRun(void * in) {}

int test_AsyncDetach(void) {
	UNIT_TEST_START;
	int result = 0;

	result = BFThreadAsyncDetach(AsyncDetachRun, NULL);

	UNIT_TEST_END(!result, result);
	return result;
}

void CancelingAsyncThreadRun(void * in) {
	sleep(5);
}

int test_CancelingAsyncThread(void) {
	UNIT_TEST_START;
	int result = 0;
	bool flag = false;

	BFThreadAsyncID id;
	if (!result) {
		id = BFThreadAsync(CancelingAsyncThreadRun, &flag);
		result = BFThreadAsyncIDError(id);
	}

	if (!result) {
		result = BFThreadAsyncCancel(id);
	}

	if (!result) {
		if (flag) result = 1;
	}
	
	BFThreadAsyncIDDestroy(id);

	UNIT_TEST_END(!result, result);
	return result;
}

int test_CancelingAsyncThreadThatHasAlreadyFinished() {
	UNIT_TEST_START;
	int result = 0;
	bool flag = false;

	BFThreadAsyncID id;
	if (!result) {
		id = BFThreadAsync(CancelingAsyncThreadRun, &flag);
		result = BFThreadAsyncIDError(id);
	}

	if (!result) {
		while (BFThreadAsyncIDIsRunning(id)) {}
		sleep(1);
		result = BFThreadAsyncCancel(id);
	}

	if (!result) {
		if (flag) result = 1;
	}
	
	BFThreadAsyncIDDestroy(id);

	UNIT_TEST_END(!result, result);
	return result;
}

void TestThreadCountThread(void * in) {

}

int test_threadCount() {
	UNIT_TEST_START;
	int result = 0;
	
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
			printf("\n%d != %d\n", BFThreadGetStartedCount(), threadcount);
		}
	}

	UNIT_TEST_END(!result, result);
	return result;
}

void thread_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	LAUNCH_TEST(test_CreatingThreadSync, p, f);
	LAUNCH_TEST(test_CreatingThreadAsync, p, f);
	LAUNCH_TEST(test_WaitingOnThreadLock, p, f);
	LAUNCH_TEST(test_LockAndUnlock, p, f);
	LAUNCH_TEST(test_ReleasingAsyncID, p, f);
	LAUNCH_TEST(test_AsyncDetach, p, f);
	LAUNCH_TEST(test_CancelingAsyncThread, p, f);
	LAUNCH_TEST(test_CancelingAsyncThreadThatHasAlreadyFinished, p, f);
	LAUNCH_TEST(test_threadCount, p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // THREAD_TESTS_H

