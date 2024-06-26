/**
 * author: Brando
 * date: 1/24/24
 */

#ifndef BUFFER_TESTS_HPP
#define BUFFER_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include <buffer.hpp>

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

int test_bufferinit() {
	UNIT_TEST_START;
	int result = 0;

	Buffer buf0;

	char bytes[10];
	Buffer buf1(sizeof(bytes), bytes);

	Buffer buf2(32);

	UNIT_TEST_END(!result, result);
	return result;
}

void buffer_tests(int * pass, int * fail) {
	int p = 0, f = 0;
	
	INTRO_TEST_FUNCTION;

	LAUNCH_TEST(test_bufferinit, p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // BUFFER_TESTS_HPP

