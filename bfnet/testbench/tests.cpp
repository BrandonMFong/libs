/**
 * author: brando
 * date: 2/15/24
 */

#include "socket_tests.hpp"
#include "log.hpp"

using namespace BF::Net;

void TestLog(const char * str) {
	printf("\nbfnet: %s\n", str);
}

int main() {
	TEST_SUITE_START;

	Log::SetCallback(TestLog);
	
	TEST_SUITE_LAUNCH(socket_tests);

	TEST_SUITE_END;

	return 0;
}

