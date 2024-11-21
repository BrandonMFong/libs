/**
 * author: Brando
 * date: 11/21/24
 */

#ifndef RAND_TESTS_H
#define RAND_TESTS_H

#include "clib_tests.h"
#include "rand.h"

BFTEST_UNIT_FUNC(test_rand, 2<<10, {
	BFRandInit(time(0));
	int a = BFRand();
	int b = BFRand();
	BF_ASSERT(a != b, "%d == %d", a, b);
})

BFTEST_COVERAGE_FUNC(rand_tests, {
	BFTEST_LAUNCH(test_rand);
})

#endif // RAND_TESTS_H

