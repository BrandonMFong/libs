/**
 * author: Brando
 * date: 11/17/24
 */

#ifndef MATH_TESTS_H
#define MATH_TESTS_H

#include "clib_tests.h"
#include "bfmath.h"
#include <math.h>
#include <time.h>

BFTEST_UNIT_FUNC(test_sqrt, 2<<10, {
	srand(time(0));
	int num = rand() % 2 << 20;
	float actual = BFMathSqrt(num);
	float expected = sqrt(num);
	BF_ASSERT(abs(actual - expected) < kBFMathSqrtFactor, "expected=%f, actual=%f", actual, expected);
})

BFTEST_UNIT_FUNC(test_gettingNthPrimeNumber, 1, {
	srand(time(0));
	int nth = rand() % 2 << 20;
	int prime = BFMathPrimeGetNumberAtIndex(nth);
	BF_ASSERT(prime != -1, "could not find prime number at primelist[%dth]", nth);
	BF_ASSERT(BFMathPrimeIsPrime(prime), "%d is not a prime number", prime);
})

BFTEST_COVERAGE_FUNC(math_tests, {
	BFTEST_LAUNCH(test_sqrt);
	BFTEST_LAUNCH(test_gettingNthPrimeNumber);

})

#endif // MATH_TESTS_H

