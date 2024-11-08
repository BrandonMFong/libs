/**
 * author: Brando
 * date: 11/7/24
 */

#ifndef SWAP_TESTS_H
#define SWAP_TESTS_H

#include "clib_tests.h"
#include "swap.h"

int test_swapping(void) {
	UNIT_TEST_START;
	int result = 0;

	int max = 2 << 21;
	while (!result && max--) {
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
	}

	UNIT_TEST_END(!result, result);
	return result;
}

void swap_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	LAUNCH_TEST(test_swapping, p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // SWAP_TESTS_H

