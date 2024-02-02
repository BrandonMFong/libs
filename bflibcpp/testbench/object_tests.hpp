/**
 * author: Brando
 * date: 2/2/24
 */

#ifndef BF_OBJECT_TESTS_HPP
#define BF_OBJECT_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include "object.hpp"

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

int test_objectinit() {
	int result = 0;

	Object o;

	PRINT_TEST_RESULTS(!result);
	return result;
}

void object_tests(int * pass, int * fail) {
	int p = 0, f = 0;
	
	INTRO_TEST_FUNCTION;

	LAUNCH_TEST(test_objectinit, p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // BF_OBJECT_TESTS_HPP

