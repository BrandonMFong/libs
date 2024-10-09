/**
 * author: Brando
 * date: 10/8/24
 */

#ifndef SOCKET_TESTS_HPP
#define SOCKET_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include <socket.hpp>
#include "bfnet_tests.hpp"

extern "C" {
//#include <bflibc/bflibc.h>
}

using namespace BF;

int test_socketinit() {
	UNIT_TEST_START;
	int result = 0;
	UNIT_TEST_END(!result, result);
	return result;
}

void socket_tests(int * pass, int * fail) {
	int p = 0, f = 0;
	
	INTRO_TEST_FUNCTION;

	LAUNCH_TEST(test_socketinit, p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // SOCKET_TESTS_HPP

