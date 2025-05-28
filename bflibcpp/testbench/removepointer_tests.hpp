/**
 * author: Brando
 * date: 5/27/25
 */

#ifndef REMOVE_POINTER_TESTS_HPP
#define REMOVE_POINTER_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include "removepointer.hpp"

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

BFTEST_UNIT_FUNC(test_removePointerInit, 2<<10, {
	srand(time(0));
	int expected = rand();
	RemovePointer<int *>::type actual = expected;
	BF_ASSERT(actual == expected);
})

BFTEST_COVERAGE_FUNC(removepointer_tests, {
	BFTEST_LAUNCH(test_removePointerInit);
})

#endif // REMOVE_POINTER_TESTS_HPP

