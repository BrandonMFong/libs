/**
 * author: Brando
 * date: 11/13/24
 */

#ifndef TESTSUITE1_TESTS_H
#define TESTSUITE1_TESTS_H 

#include "bftest.h"
#include <unistd.h>

UNIT_TEST_FUNC(test_assertsuccess) {
	UNIT_TEST_START;

	sleep(1);

	BF_ASSERT(true);

	UNIT_TEST_END;
}

UNIT_TEST_FUNC(test_assertfailure) {
	UNIT_TEST_START;

	sleep(1);

	BF_ASSERT(false);

	exit(1); // should never reach here

	UNIT_TEST_END;
}

UNIT_TEST_FUNC(test_assertfailureWithMessage) {
	UNIT_TEST_START;

	sleep(1);

	const char * msg = "hello failure!";
	BF_ASSERT(false, "%s", msg);

	exit(1); // should never reach here

	UNIT_TEST_END;
}

TEST_COVERAGE_FUNC(suite1_tests) {
	TEST_COVERAGE_START;

	LAUNCH_TEST(test_assertsuccess);
	LAUNCH_TEST(test_assertfailure);
	LAUNCH_TEST(test_assertfailureWithMessage);

	TEST_COVERAGE_END;
}

#endif // TESTSUITE1_TESTS_H

