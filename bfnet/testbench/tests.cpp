/**
 * author: brando
 * date: 2/15/24
 */

#include "socket_tests.hpp"
#include "log.hpp"

extern "C" {
#include <bflibc/bflibc.h>
#include <bflibc/bftests.h>
}

using namespace BF::Net;

void TestLog(const char * str) {
	printf("\nbfnet: %s\n", str);
}

int main() {
	TEST_SUITE_START;

	Log::SetCallback(TestLog);
	
	LAUNCH_TEST_SET(socket_tests);

	TEST_SUITE_END;

	return 0;
}

