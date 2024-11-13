/**
 * @file
 * author: Brando
 * date: 6/30/22
 */

#ifndef BF_TEST_H
#define BF_TEST_H

#include <stdio.h>
#include <stdlib.h>

#define TEST_SUITE_START \
	int pass = 0, fail = 0;\
	float tp = 0, tf = 0;

#define TEST_SUITE_LAUNCH(foo) \
	foo(&pass, &fail);\
	printf("[+ %d, - %d]\n", pass, fail);\
	tp += pass; tf += fail;\
	pass = 0; fail = 0;
 
#define TEST_SUITE_END printf("Grade - %.2f%% (%d/%d)\n", (float) ((tp/(tp+tf)) * 100), (int) tp, (int) (tp+tf));

#define INTRO_TEST_FUNCTION printf("---- %s started ----\n", __func__)

/**
 * each function should take no params and return 0 on success
 */
#define LAUNCH_TEST(foo, p, f) \
	if (!foo()) p++; \
	else f++;

#define UNIT_TEST_START \
	printf("%s - ", __func__);\
	fflush(stdout);

#define UNIT_TEST_END(success, errcode) \
	if (success) {printf("PASS\n");}\
	else {printf("FAIL %d\n", errcode);}

#endif // BF_TEST_H

