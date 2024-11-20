/**
 * author: Brando
 * date: 11/20/24
 */

#ifndef HASH_TESTS_H
#define HASH_TESTS_H

#include "clib_tests.h"
#include "hash.h"
#include "bfmath.h"

BFTEST_UNIT_FUNC(test_hashDivision, 2<<10, {
	srand(time(0));
	int key = rand();
	int prime = BFMathPrimeGetNumberAtIndex(rand() % 2 << 8);
	BFHashDivision(key, prime);
})

BFTEST_COVERAGE_FUNC(hash_tests, {
	BFTEST_LAUNCH(test_hashDivision);
})

#endif // HASH_TESTS_H

