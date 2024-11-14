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
#define TEST_SUITE_START \
	int pass = 0, fail = 0;\
	float tp = 0, tf = 0;

#define TEST_SUITE_LAUNCH(foo) \
	foo(&pass, &fail);\
	tp += pass; tf += fail;\
	pass = 0; fail = 0;
 
#define TEST_SUITE_END printf("Grade - %.2f%% (%d/%d)\n", (float) ((tp/(tp+tf)) * 100), (int) tp, (int) (tp+tf));

/** TEST COVERAGE **/

#define TEST_COVERAGE_FUNC(foo) \
	void foo (int * pass, int * fail)

#define TEST_COVERAGE_START \
	printf("---- %s started ----\n", __func__);\
	int p = 0, f = 0;

/**
 * each function should take no params and return 0 on success
 */
#define LAUNCH_TEST(foo) \
	if (!foo()) p++; \
	else f++;

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
		UNIT_TEST_END \
	}

void _BFTestLogPush(const char * format, ...);
void _BFTestLogFlush(const char * suffix);

#endif // BF_TEST_H

