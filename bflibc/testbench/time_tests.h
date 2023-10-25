/**
 * author: Brando
 * date: 3/8/23
 */

#ifndef TIME_TESTS_H
#define TIME_TESTS_H

#include "clib_tests.h"
#include <bftime.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int test_GettingTime(void) {
	UNIT_TEST_START;
	int result = 0;

	time_t b = time(NULL);
	sleep(1);
	BFTime n = BFTimeGetCurrentTime();
	sleep(1);
	time_t a = time(NULL);

	if (!((b <= n) && (n <= a))) {
		result = 1;
		printf("Failed: %ld <= %f <= %ld\n", b, n, a);
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_GettingUTCTime(void) {
	UNIT_TEST_START;
	int result = 0;
	
	BFTime n = BFTimeGetCurrentTime();
	time_t t = (time_t) n;
	struct tm tm1, tm2;

	if (gmtime_r(&t, &tm1) == NULL) result = 1;

	if (!result)
		result = BFTimeGetStructTMUTC(n, &tm2);

	if (!result) {
		time_t t1 = mktime(&tm1), t2 = mktime(&tm2);
		if (difftime(t1, t2)) result = 2;
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_GettingLocalTime(void) {
	UNIT_TEST_START;
	int result = 0;
	
	BFTime n = BFTimeGetCurrentTime();
	time_t t = (time_t) n;
	struct tm tm1, tm2;

	if (localtime_r(&t, &tm1) == NULL) result = 1;

	if (!result)
		result = BFTimeGetStructTMLocal(n, &tm2);

	if (!result) {
		time_t t1 = mktime(&tm1), t2 = mktime(&tm2);
		if (difftime(t1, t2)) result = 2;
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_GettingDateTime(void) {
	int result = 0;

	BFTime n = BFTimeGetCurrentTime();
	struct tm tm;
	time_t t = (time_t) n;
	localtime_r(&t, &tm);
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

int time_bftimesleep(void) {
	UNIT_TEST_START;
	int result = 0;
	
	BFTimeSleep(1);
	
	UNIT_TEST_END(!result, result);
	return result;
}

int time_bftimesleepmicro(void) {
	UNIT_TEST_START;
	int result = 0;
	
	BFTimeSleep(BFTimeUS(100));
	
	UNIT_TEST_END(!result, result);
	return result;
}

int time_bftimesleepmilli(void) {
	UNIT_TEST_START;
	int result = 0;
	
	BFTimeSleep(BFTimeMS(100));
	
	UNIT_TEST_END(!result, result);
	return result;
}

int time_bftimesleepsecondswithmicro(void) {
	UNIT_TEST_START;
	int result = 0;
	
	BFTimeSleep(1.123456);
	
	UNIT_TEST_END(!result, result);
	return result;
}

int time_bftimesleep0(void) {
	UNIT_TEST_START;
	int result = 0;
	
	BFTimeSleep(0);
	
	UNIT_TEST_END(!result, result);
	return result;
}

void time_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	LAUNCH_TEST(test_GettingTime, p, f);
	LAUNCH_TEST(test_GettingDateTime, p, f);
	LAUNCH_TEST(test_GettingUTCTime, p, f);
	LAUNCH_TEST(test_GettingLocalTime, p, f);
	LAUNCH_TEST(time_bftimesleep, p, f);
	LAUNCH_TEST(time_bftimesleep0, p, f);
	LAUNCH_TEST(time_bftimesleepmicro, p, f);
	LAUNCH_TEST(time_bftimesleepmilli, p, f);
	LAUNCH_TEST(time_bftimesleepsecondswithmicro, p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // TIME_TESTS_H

