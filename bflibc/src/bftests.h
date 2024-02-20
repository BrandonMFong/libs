/**
 * author: Brando
 * date: 6/30/22
 */

#ifndef BF_TESTS_H
#define BF_TESTS_H

#include <stdio.h>
#include <stdlib.h>

#define INTRO_TEST_FUNCTION printf("Running %s:\n", __func__)
#define PRINT_TEST_RESULTS(result) \
	if (result) {system("printf \"[\033[0;32m Pass \033[0m] \"");}\
	else {system("printf \"[\033[0;31m Fail \033[0m] \"");}\
	printf("%s\n", __func__)

// Use these to show progress
#define UNIT_TEST_START printf("[ .... ] %s", __func__); fflush(stdout);
#define UNIT_TEST_END(result, errcode) \
	printf("\r");fflush(stdout);\
	if (result) {system("printf \"[\033[0;32m Pass \033[0m] \"");}\
	else {system("printf \"[\033[0;31m Fail \033[0m] \"");}\
	printf("%s", __func__);\
	if (!result) printf(" - [%d]", errcode);\
	printf("\n")

/**
 * each function should take no params and return 0 on success
 */
#define LAUNCH_TEST(unit_test_function, pass_counter, fail_counter) \
	if (!unit_test_function()) pass_counter++; \
	else fail_counter++;

#define PRINT_GRADE(p, f) printf("Grade - %.2f%% (%d/%d)\n", (float) ((p/(p+f)) * 100), (int) p, (int) (p+f));

#define TEST_SUITE_START int pass = 0, fail = 0;\
						float tp = 0, tf = 0;

#define LAUNCH_TEST_SET(foo) foo(&pass, &fail);\
	printf("[+ %d, - %d]\n", pass, fail);\
	tp += pass; tf += fail;\
	pass = 0; fail = 0;
 
#define TEST_SUITE_END PRINT_GRADE(tp, tf)

#endif // BF_TESTS_H

