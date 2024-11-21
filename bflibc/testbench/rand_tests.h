/**
 * author: Brando
 * date: 11/21/24
 */

#ifndef RAND_TESTS_H
#define RAND_TESTS_H

#include "clib_tests.h"
#include "rand.h"

BFTEST_UNIT_FUNC(test_rand, 2<<15, {
	BFRandInit(time(0));
	int a = BFRand();
	int b = BFRand();
	BF_ASSERT(a != b, "%d == %d", a, b);
})

BFTEST_UNIT_FUNC(test_randdouble, 2<<15, {
	BFRandInit(time(0));
	double a = BFRand();
	double b = BFRand();
	BF_ASSERT(a != b, "%lf == %lf", a, b);
})

BFTEST_UNIT_FUNC(test_randlong, 2<<15, {
	BFRandInit(time(0));
	long a = BFRand();
	long b = BFRand();
	BF_ASSERT(a != b, "%ld == %ld", a, b);
})

BFTEST_COVERAGE_FUNC(rand_tests, {
	BFTEST_LAUNCH(test_rand);
	BFTEST_LAUNCH(test_randdouble);
	BFTEST_LAUNCH(test_randlong);
})

#endif // RAND_TESTS_H

