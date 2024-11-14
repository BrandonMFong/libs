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

/** TEST SUITE **/
// a test suite is the high level function that will call
// different sets of test coverage

/**
 * Call at the start of your test suite function
 */
#define TEST_SUITE_START \
	int pass = 0, fail = 0;\
	float tp = 0, tf = 0;

/**
 * Calls test coverage function
 */
#define TEST_SUITE_LAUNCH(foo) \
	foo(&pass, &fail);\
	tp += pass; tf += fail;\
	pass = 0; fail = 0;

/**
 * call at the end of your test suite function
 */
#define TEST_SUITE_END printf("Grade - %.2f%% (%d/%d)\n", (float) ((tp/(tp+tf)) * 100), (int) tp, (int) (tp+tf));

/** TEST COVERAGE **/

/**
 * defines the test coverage function
 */
#define TEST_COVERAGE_FUNC(foo) \
	void foo (int * pass, int * fail)

/**
 * initializes your test coverage function
 */
#define TEST_COVERAGE_START \
	printf("---- %s started ----\n", __func__);\
	int p = 0, f = 0;

/**
 * each function should take no params and return 0 on success
 */
#define LAUNCH_TEST(foo) \
	if (!foo()) p++; \
	else f++;

/**
 * ends your test your coverage function
 */
#define TEST_COVERAGE_END \
	if (pass) *pass += p;\
	if (fail) *fail += f;\
	printf("---- %s ended [+ %d, - %d] ----\n", __func__, *pass, *fail);

/** UNIT TEST **/

#define UNIT_TEST_FUNC(foo) \
	int foo (void)

#define UNIT_TEST_START \
	printf("%s - ", __func__);\
	fflush(stdout);\
	int result = 0;

#define UNIT_TEST_END \
	if (result == 0) { printf("PASS\n"); }\
	else {\
		printf("FAIL %d\n", result);\
		_BFTestLogFlush(__func__);\
	}\
	return result;

#define BF_ASSERT(expr, ...) \
	if (!(expr)) {\
		result = -1;\
		_BFTestLogPush("" __VA_ARGS__);\
		UNIT_TEST_END; \
	}

/**
 * adds a message to the test log
 */
void _BFTestLogPush(const char * format, ...);

/**
 * dumps all test log entries
 *
 * assumes all logs are error messages
 */
void _BFTestLogFlush(const char * suffix);

#endif // BF_TEST_H

