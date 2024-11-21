/**
 * author: Brando
 * date: 11/13/24
 */

#ifndef TESTSUITE1_TESTS_H
#define TESTSUITE1_TESTS_H 

#include "bftest.h"
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

BFTEST_UNIT_FUNC(test_assertsuccess, 1, {
	sleep(1);

	BF_ASSERT(true);
})

BFTEST_UNIT_FUNC(test_assertfailure, 1, {
	sleep(1);

	BF_ASSERT(false);

	exit(1); // should never reach here
})

BFTEST_UNIT_FUNC(test_assertfailureWithCondition, 1, {
	sleep(1);

	int error = -1;
	BF_ASSERT(error == 0);

	exit(1); // should never reach here
})

BFTEST_UNIT_FUNC(test_assertfailureWithMessage, 1, {
	sleep(1);

	const char * msg = "this is a failure";
	BF_ASSERT(false, "%s", msg);

	exit(1); // should never reach here
})

BFTEST_UNIT_FUNC(test_assertfailureWithMessageAndCondition, 1, {
	sleep(1);

	const char * msg = "this is a failure";
	int error = -1;
	BF_ASSERT(error == 0, "%s", msg);

	exit(1); // should never reach here
})

BFTEST_UNIT_FUNC(test_testLogging, 1, {
	sleep(1);
	BFTestPrint("this is a log message");
	BFTestPrint("this is another log message");
	srand(time(0));
	int num = rand();
	BFTestPrint("this is one with a random number: %d", num);
	BFTestPrint("this is the last one");
})

BFTEST_COVERAGE_FUNC(suite1_tests, {
	BFTEST_LAUNCH(test_assertsuccess);
	BFTEST_LAUNCH(test_assertfailure);
	BFTEST_LAUNCH(test_assertfailureWithMessage);
	BFTEST_LAUNCH(test_assertfailureWithMessageAndCondition);
	BFTEST_LAUNCH(test_assertfailureWithCondition);
	BFTEST_LAUNCH(test_testLogging);
})

#endif // TESTSUITE1_TESTS_H

