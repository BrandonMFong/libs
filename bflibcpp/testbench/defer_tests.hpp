/**
 * author: Brando
 * date: 4/20/25
 */

#ifndef DEFER_TESTS_HPP
#define DEFER_TESTS_HPP

#include <defer.hpp>
#include "cpplib_tests.hpp"

using namespace BF;

BFTEST_UNIT_FUNC(test_deferInit, 2<<10, {
	int * var = new int;
	__Defer__ d([=] () {
		delete var;
	});

	srand(time(0));
	*var = rand();
})

BFTEST_UNIT_FUNC(test_deferMacro, 2<<10, {
	int * var = new int;
	BFDefer([=] () {
		delete var;
	});

	srand(time(0));
	*var = rand();
})

BFTEST_UNIT_FUNC(test_deferOrder, 2<<10, {
	int val = 0;
	BFDefer([&] () {
		if (BFTEST_UNIT_FUNC_ITR == 0) {
			BFTestPrint("%d ?= 1024 -> %s", val, val == 1024 ? "yes" : "no");
		}
	});
	val = 1024;
})

BFTEST_COVERAGE_FUNC(defer_tests, {
	BFTEST_LAUNCH(test_deferInit);
	BFTEST_LAUNCH(test_deferMacro);
	BFTEST_LAUNCH(test_deferOrder);
})

#endif // DEFER_TESTS_HPP

