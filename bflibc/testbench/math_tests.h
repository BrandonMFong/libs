/**
 * author: Brando
 * date: 11/17/24
 */

#ifndef MATH_TESTS_H
#define MATH_TESTS_H

#include "clib_tests.h"
#include "bfmath.h"
#include "rand.h"
#include <math.h>
#include <time.h>

BFTEST_UNIT_FUNC(test_sqrt, 2<<10, {
	srand(time(0));
	int num = rand() % 2 << 10;
	float actual = BFMathSqrt(num);
	float expected = sqrt(num);
	BF_ASSERT(abs(actual - expected) < kBFMathSqrtFactor, "expected=%f, actual=%f", actual, expected);
})

BFTEST_UNIT_FUNC(test_gettingNthPrimeNumber, 2, {
	srand(time(0));
	int nth = rand() % 2 << 10;
	int prime = BFMathPrimeGetNumberAtIndex(nth);
	BF_ASSERT(prime != -1, "could not find prime number at primelist[%dth]", nth);
	BF_ASSERT(BFMathPrimeIsPrime(prime), "%d is not a prime number", prime);
})

BFTEST_UNIT_FUNC(test_gettingMaxFor2Integers, 2<<3, {
	BFRandInit(time(0));
	int a = BFRand();
	int b = BFRand();
	BFTestPrint("max(%d, %d)", a, b);
	int max = BFMathMax(a, b);
	int expect = a > b ? a : b;
	BF_ASSERT(max == expect, "max(%d, %d)=%d expect=%d", a, b, max, expect);

	BFTestPrint("min(%d, %d)", a, b);
	int min = BFMathMin(a, b);
	expect = a < b ? a : b;
	BF_ASSERT(min == expect, "min(%d, %d)=%d expect=%d", a, b, min, expect);
})

BFTEST_UNIT_FUNC(test_gettingMaxFor2Doubles, 2<<3, {
	BFRandInit(time(0));
	double a = BFRandDouble();
	double b = BFRandDouble();
	BFTestPrint("max(%lf, %lf)", a, b);
	double max = BFMathMax(a, b);
	double expect = a > b ? a : b;
	BF_ASSERT(max == expect, "max(%lf, %lf)=%lf expect=%lf", a, b, max, expect);
	
	BFTestPrint("min(%lf, %lf)", a, b);
	double min = BFMathMin(a, b);
	expect = a < b ? a : b;
	BF_ASSERT(min == expect, "min(%lf, %lf)=%lf expect=%lf", a, b, min, expect);
})

BFTEST_UNIT_FUNC(test_gettingMaxFor2Longs, 2<<3, {
	BFRandInit(time(0));
	long a = BFRand();
	long b = BFRand();
	BFTestPrint("max(%ld, %ld)", a, b);
	long max = BFMathMax(a, b);
	long expect = a > b ? a : b;
	BF_ASSERT(max == expect, "max(%ld, %ld)=%ld expect=%ld", a, b, max, expect);
	
	BFTestPrint("min(%ld, %ld)", a, b);
	long min = BFMathMin(a, b);
	expect = a < b ? a : b;
	BF_ASSERT(min == expect, "min(%ld, %ld)=%ld expect=%ld", a, b, min, expect);
})

BFTEST_COVERAGE_FUNC(math_tests, {
	BFTEST_LAUNCH(test_sqrt);
	BFTEST_LAUNCH(test_gettingNthPrimeNumber);
	BFTEST_LAUNCH(test_gettingMaxFor2Integers);
	BFTEST_LAUNCH(test_gettingMaxFor2Doubles);
	BFTEST_LAUNCH(test_gettingMaxFor2Longs);
})

#endif // MATH_TESTS_H

