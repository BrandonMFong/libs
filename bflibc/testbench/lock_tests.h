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

void lock_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	LAUNCH_TEST(test_CreatingBFLock, p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // LOCK_TESTS_H

