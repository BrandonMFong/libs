/**
 * author: Brando
 * date: 11/13/24
 */

#ifndef TESTSUITE1_TESTS_H
#define TESTSUITE1_TESTS_H 

#include "bftest.h"
#include <unistd.h>

BFTEST_UNIT_FUNC(test_assertsuccess) {
	BFTEST_UNIT_START;

	sleep(1);

	BF_ASSERT(true);

	BFTEST_UNIT_END;
}

BFTEST_UNIT_FUNC(test_assertfailure) {
	BFTEST_UNIT_START;

	sleep(1);

	BF_ASSERT(false);

	exit(1); // should never reach here

	BFTEST_UNIT_END;
}

BFTEST_UNIT_FUNC(test_assertfailureWithMessage) {
	BFTEST_UNIT_START;

	sleep(1);

	const char * msg = "this is a failure";
	BF_ASSERT(false, "%s", msg);

	exit(1); // should never reach here

	BFTEST_UNIT_END;
}

BFTEST_COVERAGE_FUNC(suite1_tests) {
	BFTEST_COVERAGE_START;

	BFTEST_LAUNCH(test_assertsuccess);
	BFTEST_LAUNCH(test_assertfailure);
	BFTEST_LAUNCH(test_assertfailureWithMessage);

	BFTEST_COVERAGE_END;
}

#endif // TESTSUITE1_TESTS_H

