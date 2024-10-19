/**
 * author: Brando
 * date: 10/25/23
 */

#ifndef LOCK_TESTS_H
#define LOCK_TESTS_H

#include "clib_tests.h"
#include "lock.h"

int test_CreatingBFLock(void) {
	UNIT_TEST_START;
	int result = 0;

	BFLock lock;
	result = BFLockCreate(&lock);

	if (!result) {
		if (!BFLockIsValid(&lock)) result = 1;
	}

	if (!result) result = BFLockDestroy(&lock);

	UNIT_TEST_END(!result, result);
	return result;
}

int test_CreatingTimedWaitLock(void) {
	UNIT_TEST_START;
	int result = 0;

	BFLock lock;
	result = BFLockCreate(&lock);

	if (!result) {
		if (!BFLockIsValid(&lock)) result = 1;
	}

	if (!result) {
		result = BFLockTimedWait(&lock, 2); // should simply sleep for 2 seconds

		if (result == kBFLockTimedWaitCodeTimedOut)
			result = 0;
	}

	if (!result) result = BFLockDestroy(&lock);

	UNIT_TEST_END(!result, result);
	return result;
}

typedef struct {
	BFLock lock;
	int i;
} thread_test_waitinglock_struct;

void thread_test_waitinglock(void * in) {
	thread_test_waitinglock_struct * st = (thread_test_waitinglock_struct *) in;

	// this shouldn't work
	//BFLockLock(&st->lock);
	st->i++;
	//BFLockUnlock(&st->lock);

	BFLockRelease(&st->lock);
}

int test_waitinglock() {
	UNIT_TEST_START;
	int result = 0;

	int max = 2 << 4;
	while (!result && max--) {
		thread_test_waitinglock_struct st;
		st.i = 0;
		result = BFLockCreate(&st.lock);

		if (!result) {
			if (!BFLockIsValid(&st.lock)) result = 1;
		}

		if (!result) {
			BFThreadAsyncID tid = BFThreadAsync(thread_test_waitinglock, &st);
			while (!BFThreadAsyncIsRunning(tid)) { usleep(50); }
			BFLockWait(&st.lock);

			BFThreadAsyncWait(tid);

			if (st.i == 0) {
				result = 1;
			}
			BFThreadAsyncDestroy(tid);
		}

		if (!result) result = BFLockDestroy(&st.lock);
	}

	UNIT_TEST_END(!result, result);
	return result;

}

typedef struct {
	BFLock l;
	bool ran;
	int err;
} test_destroyLockThatIsWaiting_struct;

void thread_test_destroyLockThatIsWaiting(void * in) {
	test_destroyLockThatIsWaiting_struct * st = (test_destroyLockThatIsWaiting_struct *) in;
	st->err = BFLockWait(&st->l);
	st->ran = true;
}

int test_destroyLockThatIsWaiting() {
	UNIT_TEST_START;
	int result = 0;

	int max = 2 << 14;
	while (!result && max--) {
		test_destroyLockThatIsWaiting_struct st;
		BFLockCreate(&st.l);
		st.ran = false;
		st.err = 0;

		BFThreadAsyncID tid = BFThreadAsync(thread_test_destroyLockThatIsWaiting, &st);
		while (!BFThreadAsyncIsRunning(tid)) { usleep(50); }

		while (!BFLockIsWaiting(&st.l)) {
			usleep(50);
		}

		BFLockDestroy(&st.l);

		BFThreadAsyncWait(tid);
		BFThreadAsyncDestroy(tid);

		if (!st.ran) {
			result = 1;
		}

		if (st.err) {
			result += 2;
		}
	}

	UNIT_TEST_END(!result, result);
	return result;

}

void lock_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	LAUNCH_TEST(test_CreatingBFLock, p, f);
	LAUNCH_TEST(test_CreatingTimedWaitLock, p, f);
	LAUNCH_TEST(test_waitinglock, p, f);
	LAUNCH_TEST(test_destroyLockThatIsWaiting, p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // LOCK_TESTS_H

