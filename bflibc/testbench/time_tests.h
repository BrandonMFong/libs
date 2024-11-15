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

//int test_GettingTime(void) {
BFTEST_UNIT_FUNC(test_GettingTime, 1, {
	time_t b = time(NULL);
	sleep(1);
	BFTime n = BFTimeGetCurrentTime();
	sleep(1);
	time_t a = time(NULL);

	if (!((b <= n) && (n <= a))) {
		result = 1;
		printf("Failed: %ld <= %f <= %ld\n", b, n, a);
	}
})

//int test_GettingUTCTime(void) {
BFTEST_UNIT_FUNC(test_GettingUTCTime, 1, {
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
})

//int test_GettingLocalTime(void) {
BFTEST_UNIT_FUNC(test_GettingLocalTime, 1, {
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
})

//int test_GettingDateTime(void) {
BFTEST_UNIT_FUNC(test_GettingDateTime, 1, {
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
})

//int time_bftimesleep(void) {
BFTEST_UNIT_FUNC(time_bftimesleep, 1, {
	BFTimeSleep(1);
})

//int time_bftimesleepmicro(void) {
BFTEST_UNIT_FUNC(time_bftimesleepmicro, 1, {
	BFTimeSleep(BFTimeUS(100));
})

//int time_bftimesleepmilli(void) {
BFTEST_UNIT_FUNC(time_bftimesleepmilli, 1, {
	BFTimeSleep(BFTimeMS(100));
})

//int time_bftimesleepsecondswithmicro(void) {
BFTEST_UNIT_FUNC(time_bftimesleepsecondswithmicro, 1, {
	BFTimeSleep(1.123456);
})

//int time_bftimesleep0(void) {
BFTEST_UNIT_FUNC(time_bftimesleep0, 1, {
	BFTimeSleep(0);
})

BFTEST_COVERAGE_FUNC(time_tests, {
	BFTEST_LAUNCH(test_GettingTime);
	BFTEST_LAUNCH(test_GettingDateTime);
	BFTEST_LAUNCH(test_GettingUTCTime);
	BFTEST_LAUNCH(test_GettingLocalTime);
	BFTEST_LAUNCH(time_bftimesleep);
	BFTEST_LAUNCH(time_bftimesleep0);
	BFTEST_LAUNCH(time_bftimesleepmicro);
	BFTEST_LAUNCH(time_bftimesleepmilli);
	BFTEST_LAUNCH(time_bftimesleepsecondswithmicro);
})

#endif // TIME_TESTS_H

