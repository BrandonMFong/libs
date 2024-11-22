/**
 * author: Brando
 * date: 11/20/24
 */

#ifndef HASH_TESTS_H
#define HASH_TESTS_H

#include "clib_tests.h"
#include "hash.h"
#include "bfmath.h"
#include "rand.h"

BFTEST_UNIT_FUNC(test_hashDivision, 2<<10, {
	BFRandInit(time(0));
	int key = BFRand();
	int prime = BFMathPrimeGetNumberAtIndex(BFRand() % 2 << 8);
	BFHashDivision(key, prime);
})

BFTEST_UNIT_FUNC(test_hashMultiplication, 2<<10, {
	BFRandInit(time(0));
	int key = BFRand();
	int prime = BFMathPrimeGetNumberAtIndex(BFRand() % 2 << 8);
	double fractional = BFRandDouble();
	fractional = fractional - ((int) fractional);
	
	//int BFHashMultiplication(long long key, double fractional, long long prime);
	BFHashMultiplication(key, fractional, prime);
})

BFTEST_COVERAGE_FUNC(hash_tests, {
	BFTEST_LAUNCH(test_hashDivision);
})

#endif // HASH_TESTS_H

