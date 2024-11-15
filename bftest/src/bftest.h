/**
 * @file
 * author: Brando
 * date: 6/30/22
 */

#ifndef BF_TEST_H
#define BF_TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

/** TEST SUITE **/
// a test suite is the high level function that will call
// different sets of test coverage

#define BFTEST_SUITE_FUNC(...) \
	int main () {\
		int pass = 0, fail = 0;\
		float tp = 0, tf = 0;\
		__VA_ARGS__ \
		printf("Grade - %.2f%% (%d/%d)\n", (float) ((tp/(tp+tf)) * 100), (int) tp, (int) (tp+tf));\
		return 0;\
	}

/*
 * Calls test coverage function
 */
#define BFTEST_SUITE_LAUNCH(foo) \
	foo(&pass, &fail);\
	tp += pass; tf += fail;\
	pass = 0; fail = 0;

/** TEST COVERAGE **/

/**
 * defines the test coverage function
 */
#define BFTEST_COVERAGE_FUNC(foo, ...) \
	void foo (int * pass, int * fail) {\
		printf("---- %s started ----\n", __func__);\
		int p = 0, f = 0;\
		__VA_ARGS__ \
		if (pass) *pass += p;\
		if (fail) *fail += f;\
		printf("---- %s ended [+ %d, - %d] ----\n", __func__, *pass, *fail);\
	}
/**
 * each function should take no params and return 0 on success
 */
#define BFTEST_LAUNCH(foo) \
	if (!foo()) p++; \
	else f++;

/** UNIT TEST **/

#define BFTEST_UNIT_FUNC(name, repeat, ...) \
	int name (void) {\
		BFTEST_UNIT_START;\
		int max = repeat;\
		while (!result && max--) {\
			usleep(50);\
			__VA_ARGS__ \
		}\
		BFTEST_UNIT_END;\
	}

/**
 * `result` is defined here and can be used in unit test functions
 * it is advisable to refrain from setting `result` and use
 * `BF_ASSERT`
 *
 * NOTE: using `result` is scheduled to be deprecated
 */
#define BFTEST_UNIT_START \
	printf("%s - ", __func__);\
	fflush(stdout);\
	int result = 0;

#define BFTEST_UNIT_END \
	if (result == 0) { printf("PASS\n"); }\
	else {\
		printf("FAIL\n");\
		_BFTestLogFlush();\
	}\
	return result;

/** ASSERTS **/

/**
 * exits function if expr fails then logs event
 */
#define BF_ASSERT(expr, ...) \
	if (!(expr)) {\
		result = -1;\
		_BFTestLogPush(\
			__FILE__,\
			__LINE__,\
			_kBFTestLogTypeAssertFailure,\
			#expr,\
			"" __VA_ARGS__\
		);\
		BFTEST_UNIT_END; \
	}

typedef enum _BFTestLogType {
	_kBFTestLogTypeAssertFailure = 0,
} _BFTestLogType;

/**
 * adds a message to the test log
 * 
 * all messages will get printed after the unit
 * test finishes
 */
void _BFTestLogPush(
	const char * filename,
	int line,
	_BFTestLogType logtype,
	const char * expression,
	const char * format,
	...
);

/**
 * dumps all test log entries
 *
 * assumes all logs are error messages
 */
void _BFTestLogFlush();

#endif // BF_TEST_H

