/**
 * author: Brando
 * date: 4/20/25
 */

#ifndef DEFER_TESTS_HPP
#define DEFER_TESTS_HPP

#include <defer.hpp>
#include "cpplib_tests.hpp"

using namespace BF;

BFTEST_UNIT_FUNC(test_deferInit, 1, {
	int * var = new int;
	__Defer__ d([=] () {
		delete var;
	});
})

BFTEST_COVERAGE_FUNC(defer_tests, {
	BFTEST_LAUNCH(test_deferInit);
})

#endif // DEFER_TESTS_HPP

