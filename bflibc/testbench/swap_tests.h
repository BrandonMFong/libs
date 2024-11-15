/**
 * author: Brando
 * date: 11/7/24
 */

#ifndef SWAP_TESTS_H
#define SWAP_TESTS_H

#include "clib_tests.h"
#include "swap.h"

BFTEST_UNIT_FUNC(test_swapping, 2<<10, {
	srand(time(0));
	int a, b, c, d;
	a = b = rand();
	c = d = rand();
	BFSwap(a, c);
	if (a != d) {
		result = 1;
	} else if (c != b) {
		result = 2;
	}

	if (!result) {
		a = b = c = d = rand();
		BFSwap(a, c);
		if (a != d) {
			result = 3;
		} else if (c != b) {
			result = 4;
		}
	}
})

BFTEST_COVERAGE_FUNC(swap_tests, {
	BFTEST_LAUNCH(test_swapping);
})

#endif // SWAP_TESTS_H

