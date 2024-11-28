/**
 * author: Brando
 * date: 7/11/24
 */

#ifndef EXCEPTION_TESTS_HPP
#define EXCEPTION_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include "exception.hpp"

extern "C" {
#include <bflibc/bflibc.h>
#include <bftest/bftest.h>
}

using namespace BF;

BFTEST_UNIT_FUNC(test_exceptions, 2<<10, {
	try {
		throw Exception("hello world! i threw an exception! %d", max);
	} catch (Exception & e) {
		BF_ASSERT(e.what() != NULL);
		BF_ASSERT(strlen(e.what()) > 0);
	}
})

BFTEST_COVERAGE_FUNC(exception_tests, {
	BFTEST_LAUNCH(test_exceptions);
})

#endif // EXCEPTION_TESTS_HPP

