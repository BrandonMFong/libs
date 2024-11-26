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

BFTEST_UNIT_FUNC(test_sqrt, 2<<11, {
	if (BFTEST_UNIT_FUNC_ITR == 0) {
		BFRandInit(time(0));
	}

	int num = abs(BFRand()) % (2 << 5);
	float actual = BFMathSqrt(num);
	float expected = sqrt(num);
	BF_ASSERT(abs(actual - expected) <= kBFMathSqrtFactor, "sqrt(%d) expected=%f, actual=%f", num, expected, actual);
})

BFTEST_UNIT_FUNC(test_ifZeroAndOneArePrimeNumbers, 2<<10, {
	BF_ASSERT(!BFMathPrimeIsPrime(0), "zero is not a prime number");
	BF_ASSERT(!BFMathPrimeIsPrime(1), "one is not a prime number");
})

BFTEST_UNIT_FUNC(test_gettingSmallNthPrimeNumber, 1, {
	if (BFTEST_UNIT_FUNC_ITR == 0) {
		BFRandInit(time(0));
	}
	//int nth = abs(BFRand()) % kBFMathPrimeCachedPrimesCount;
	int nth = abs(BFRand()) % (2<<5);

	int prime = BFMathPrimeGetNumberAtIndex(nth);
	BF_ASSERT(prime != -1, "could not find prime number at primelist[%dth]", nth);
	BF_ASSERT(BFMathPrimeIsPrime(prime), "prime[%d] = %d is not a prime number", nth, prime);
})

BFTEST_UNIT_FUNC(test_gettingMaxFor2Integers, 2<<11, {
	BFRandInit(time(0));
	int a = BFRand();
	int b = BFRand();
	int max = BFMathMax(a, b);
	int expect = a > b ? a : b;
	BF_ASSERT(max == expect, "max(%d, %d)=%d expect=%d", a, b, max, expect);

	int min = BFMathMin(a, b);
	expect = a < b ? a : b;
	BF_ASSERT(min == expect, "min(%d, %d)=%d expect=%d", a, b, min, expect);
})

BFTEST_UNIT_FUNC(test_gettingMaxFor2Doubles, 2<<11, {
	BFRandInit(time(0));
	double a = BFRandDouble();
	double b = BFRandDouble();
	double max = BFMathMax(a, b);
	double expect = a > b ? a : b;
	BF_ASSERT(max == expect, "max(%lf, %lf)=%lf expect=%lf", a, b, max, expect);
	
	double min = BFMathMin(a, b);
	expect = a < b ? a : b;
	BF_ASSERT(min == expect, "min(%lf, %lf)=%lf expect=%lf", a, b, min, expect);
})

BFTEST_UNIT_FUNC(test_gettingMaxFor2Longs, 2<<11, {
	BFRandInit(time(0));
	long a = BFRand();
	long b = BFRand();
	long max = BFMathMax(a, b);
	long expect = a > b ? a : b;
	BF_ASSERT(max == expect, "max(%ld, %ld)=%ld expect=%ld", a, b, max, expect);
	
	long min = BFMathMin(a, b);
	expect = a < b ? a : b;
	BF_ASSERT(min == expect, "min(%ld, %ld)=%ld expect=%ld", a, b, min, expect);
})

BFTEST_UNIT_FUNC(test_gettingMaxFor3Integers, 2<<11, {
	BFRandInit(time(0));
	int a = BFRand();
	int b = BFRand();
	int c = BFRand();
	int max = BFMathMax(a, b, c);
	int expect = a > b ? ((a > c) ? a : c) : ((b > c) ? b : c);
	BF_ASSERT(max == expect, "max(%d, %d, %d)=%d expect=%d", a, b, c, max, expect);

	int min = BFMathMin(a, b, c);
	expect = a < b ? ((a < c) ? a : c) : ((b < c) ? b : c);
	BF_ASSERT(min == expect, "min(%d, %d, %d)=%d expect=%d", a, b, c, min, expect);
})

BFTEST_UNIT_FUNC(test_gettingMaxFor3Doubles, 2<<11, {
	BFRandInit(time(0));
	double a = BFRandDouble();
	double b = BFRandDouble();
	double c = BFRandDouble();
	double max = BFMathMax(a, b, c);
	double expect = a > b ? ((a > c) ? a : c) : ((b > c) ? b : c);
	BF_ASSERT(max == expect, "max(%lf, %lf, %lf)=%lf expect=%lf", a, b, c, max, expect);

	double min = BFMathMin(a, b, c);
	expect = a < b ? ((a < c) ? a : c) : ((b < c) ? b : c);
	BF_ASSERT(min == expect, "min(%lf, %lf, %lf)=%lf expect=%lf", a, b, c, min, expect);
})

BFTEST_UNIT_FUNC(test_gettingMaxFor3Longs, 2<<11, {
	BFRandInit(time(0));
	long a = BFRand();
	long b = BFRand();
	long c = BFRand();
	long max = BFMathMax(a, b, c);
	long expect = a > b ? ((a > c) ? a : c) : ((b > c) ? b : c);
	BF_ASSERT(max == expect, "max(%ld, %ld, %ld)=%ld expect=%ld", a, b, c, max, expect);

	long min = BFMathMin(a, b, c);
	expect = a < b ? ((a < c) ? a : c) : ((b < c) ? b : c);
	BF_ASSERT(min == expect, "min(%ld, %ld, %ld)=%ld expect=%ld", a, b, c, min, expect);
})

#define __comp__(type, ap, bp) ({\
	type a = *(type*)ap;\
	type b = *(type*)bp;\
	type res;\
	if (a == b) {\
		res = 0;\
	} else if (a < b) {\
		res = -1;\
	} else {\
		res = 1;\
	}\
	res;\
})

int comp(const void * ap, const void * bp) {
	return __comp__(int, ap, bp);
}

int dcomp(const void * ap, const void * bp) {
	return __comp__(double, ap, bp);
}

int lcomp(const void * ap, const void * bp) {
	return __comp__(long, ap, bp);
}

BFTEST_UNIT_FUNC(test_gettingMaxFor10Integers, 2<<10, {
	BFRandInit(time(0));
	int a = BFRand();
	int b = BFRand();
	int c = BFRand();
	int d = BFRand();
	int e = BFRand();
	int f = BFRand();
	int g = BFRand();
	int h = BFRand();
	int i = BFRand();
	int j = BFRand();

	int arr[10] = {a, b, c, d, e, f, g, h, i, j};
	qsort(arr, 10, sizeof(int), comp);

	int max = BFMathMax(a, b, c, d, e, f, g, h, i, j);
	int expect = arr[9];
	BF_ASSERT(max == expect, "max(%d, %d, %d, %d, %d, %d, %d, %d, %d, %d)=%d expect=%d", a, b, c, d, e, f, g, h, i, j, max, expect);

	int min = BFMathMin(a, b, c, d, e, f, g, h, i, j);
	expect = arr[0];
	BF_ASSERT(min == expect, "min(%d, %d, %d, %d, %d, %d, %d, %d, %d, %d)=%d expect=%d", a, b, c, d, e, f, g, h, i, j, min, expect);
})

BFTEST_UNIT_FUNC(test_gettingMaxFor10Doubles, 2<<10, {
	BFRandInit(time(0));
	double a = BFRandDouble();
	double b = BFRandDouble();
	double c = BFRandDouble();
	double d = BFRandDouble();
	double e = BFRandDouble();
	double f = BFRandDouble();
	double g = BFRandDouble();
	double h = BFRandDouble();
	double i = BFRandDouble();
	double j = BFRandDouble();

	double arr[10] = {a, b, c, d, e, f, g, h, i, j};
	qsort(arr, 10, sizeof(double), dcomp);

	double max = BFMathMax(a, b, c, d, e, f, g, h, i, j);
	double expect = arr[9];
	BF_ASSERT(max == expect, "max(%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf)=%lf expect=%lf", a, b, c, d, e, f, g, h, i, j, max, expect);

	double min = BFMathMin(a, b, c, d, e, f, g, h, i, j);
	expect = arr[0];
	BF_ASSERT(min == expect, "min(%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf)=%lf expect=%lf", a, b, c, d, e, f, g, h, i, j, min, expect);
})

BFTEST_UNIT_FUNC(test_gettingMaxFor10Longs, 2<<10, {
	BFRandInit(time(0));
	long a = BFRandLong();
	long b = BFRandLong();
	long c = BFRandLong();
	long d = BFRandLong();
	long e = BFRandLong();
	long f = BFRandLong();
	long g = BFRandLong();
	long h = BFRandLong();
	long i = BFRandLong();
	long j = BFRandLong();

	long arr[10] = {a, b, c, d, e, f, g, h, i, j};
	qsort(arr, 10, sizeof(long), lcomp);

	long max = BFMathMax(a, b, c, d, e, f, g, h, i, j);
	long expect = arr[9];
	BF_ASSERT(max == expect, "max(%ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld)=%ld expect=%ld", a, b, c, d, e, f, g, h, i, j, max, expect);

	long min = BFMathMin(a, b, c, d, e, f, g, h, i, j);
	expect = arr[0];
	BF_ASSERT(min == expect, "min(%ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld)=%ld expect=%ld", a, b, c, d, e, f, g, h, i, j, min, expect);
})

BFTEST_UNIT_FUNC(test_absoluteValue, 2<<11, {
	for (int i = -1; i > (-1 * (2<<10)); i--) {
		int value = abs(BFRand()) * -1;
		int a = BFMathAbs(value);
		int b = abs(value);
		BF_ASSERT(a == b, "%d != %d", a, b);
	}
})

BFTEST_UNIT_FUNC(test_absoluteValueDouble, 2<<11, {
	for (int i = -1; i > (-1 * (2<<10)); i--) {
		double value = abs(BFRandDouble()) * -1;
		int a = BFMathAbs(value);
		int b = abs(value);
		BF_ASSERT(a == b, "%d != %d", a, b);
	}
})

BFTEST_COVERAGE_FUNC(math_tests, {
	BFTEST_LAUNCH(test_sqrt);
	BFTEST_LAUNCH(test_ifZeroAndOneArePrimeNumbers);
	BFTEST_LAUNCH(test_gettingSmallNthPrimeNumber);
	BFTEST_LAUNCH(test_gettingMaxFor2Integers);
	BFTEST_LAUNCH(test_gettingMaxFor2Doubles);
	BFTEST_LAUNCH(test_gettingMaxFor2Longs);
	BFTEST_LAUNCH(test_gettingMaxFor3Integers);
	BFTEST_LAUNCH(test_gettingMaxFor3Doubles);
	BFTEST_LAUNCH(test_gettingMaxFor3Longs);
	BFTEST_LAUNCH(test_gettingMaxFor10Integers);
	BFTEST_LAUNCH(test_gettingMaxFor10Doubles);
	BFTEST_LAUNCH(test_gettingMaxFor10Longs);
	BFTEST_LAUNCH(test_absoluteValue);
	BFTEST_LAUNCH(test_absoluteValueDouble);

})

#endif // MATH_TESTS_H

