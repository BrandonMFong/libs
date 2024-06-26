/**
 * author: Brando
 * date: 1/24/24
 */

#ifndef DATA_TESTS_HPP
#define DATA_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include <data.hpp>

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

int test_datainit() {
	UNIT_TEST_START;
	int result = 0;

	Data buf0;

	unsigned char bytes[10];
	Data buf1(sizeof(bytes), bytes);

	Data buf2(32);

	UNIT_TEST_END(!result, result);
	return result;
}

void data_tests(int * pass, int * fail) {
	int p = 0, f = 0;
	
	INTRO_TEST_FUNCTION;

	LAUNCH_TEST(test_datainit, p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // DATA_TESTS_HPP

