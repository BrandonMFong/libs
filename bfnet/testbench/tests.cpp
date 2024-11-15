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

BFTEST_SUITE_FUNC({
	Log::SetCallback(TestLog);
	BFTEST_SUITE_LAUNCH(socket_tests);
})

