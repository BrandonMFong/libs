/**
 * author: brando
 * date: 2/15/24
 */

#include "socket_tests.hpp"

extern "C" {
#include <bflibc/bflibc.h>
#include <bflibc/bftests.h>
}

int main() {
	TEST_SUITE_START;
	
	LAUNCH_TEST_SET(socket_tests);

	TEST_SUITE_END;

	return 0;
}

