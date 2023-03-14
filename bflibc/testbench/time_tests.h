/**
 * author: Brando
 * date: 3/8/23
 */

#ifndef TIME_TESTS_H
#define TIME_TESTS_H

#include "clib_tests.h"
#include <bftime.h>
#include <stdio.h>

int test_GettingTime(void) {
	int result = 0;

	time_t b = time(NULL);
	BFTime n = BFTimeGetCurrentTime();
	time_t a = time(NULL);

	if (!((b <= n) && (n <= a))) {
		result = 1;
		printf("Failed: %ld <= %ld <= %ld\n", b, n, a);
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_GettingDateTime(void) {
	int result = 0;

	BFTime n = BFTimeGetCurrentTime();
	struct tm tm;
	localtime_r(&n, &tm);
	BFDateTime dt = {0};

	if (n == 0) {
		result = 1;
	}

	if (result == 0) {
		if (BFTimeGetCurrentDateTime(&dt)) {
			result = 2;
			printf("Error getting datetime\n");
		} else if (dt.year != (tm.tm_year + 1900)) {
			result = 3;
		} else if (dt.month != (tm.tm_mon + 1)) {
			result = 4;
		} else if (dt.day != (tm.tm_mday)) {
			result = 5;
		} else if (dt.hour != (tm.tm_hour)) {
			result = 6;
		} else if (dt.minute != (tm.tm_min)) {
			result = 7;
		} else if (dt.second != (tm.tm_sec)) {
			result = 8;
		}
	}

	if (result) {
		printf("Error: %d\n", result);
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

void time_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (!test_GettingTime()) p++;
	else f++;

	if (!test_GettingDateTime()) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // TIME_TESTS_H

