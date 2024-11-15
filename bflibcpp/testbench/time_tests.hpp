/**
 * author: Brando
 * date: 6/30/22
 */

#ifndef TIME_TESTS_HPP
#define TIME_TESTS_HPP

#include <time.hpp>

extern "C" {
#include <bflibc/bflibc.h>
#include <unistd.h>
#include <bftest/bftest.h>
}

using namespace BF;

//int test_TimeInit() {
BFTEST_UNIT_FUNC(test_TimeInit, 1,  {
	BFTime t = BFTimeGetCurrentTime();
	Time * tm = new Time(t);

	if (tm == NULL) {
		result = 1;
	} else if (tm->epoch() != t) {
		result = 2;
	}

	delete(tm);

	Time tm2(t);

	if (result == 0) {
		if (tm2.epoch() != t) {
			result = 3;
		}
	}

	Time tm3;
	if (result == 0) {
		if (tm3.epoch() != 0) {
			result = 4;
		}
	}
	
	if (result == 0) {
		tm3 = tm2;
		if (tm3 != tm2) {
			result = 4;
		}
	}

	if (result == 0) {
		t = BFTimeGetCurrentTime();
		tm3 = t;
		if (tm3.epoch() != t) {
			result = 4;
		}
	}
})

//int test_TimeBasicOperations() {
BFTEST_UNIT_FUNC(test_TimeBasicOperations, 1,  {
	BFTime a = BFTimeGetCurrentTime();
	sleep(1);
	BFTime b = BFTimeGetCurrentTime();

	Time ta(a), tb(b);

	Time * tc = tb - ta;

	delete(tc);
})

//void time_tests(int * pass, int * fail) {
BFTEST_COVERAGE_FUNC(time_tests, {
	BFTEST_LAUNCH(test_TimeInit);
	BFTEST_LAUNCH(test_TimeBasicOperations);
})

#endif // TIME_TESTS_HPP

