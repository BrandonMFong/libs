/**
 * author: Brando
 * date: 6/30/22
 */

#ifndef TIME_TESTS_HPP
#define TIME_TESTS_HPP

#include <time.hpp>
#include "cpplib_tests.hpp"
#include <delete.hpp>

extern "C" {
#include <bflibc/bflibc.h>
#include <unistd.h>
}

using namespace BF;

int test_TimeInit() {
	int result = 0;
	BFTime t = BFTimeGetCurrentTime();
	Time * tm = new Time(t);

	if (tm == NULL) {
		result = 1;
	} else if (tm->epoch() != t) {
		result = 2;
	}

	Delete(tm);

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

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_TimeBasicOperations() {
	int result = 0;

	BFTime a = BFTimeGetCurrentTime();
	sleep(1);
	BFTime b = BFTimeGetCurrentTime();

	Time ta(a), tb(b);

	Time * tc = tb - ta;

	Delete(tc);
	PRINT_TEST_RESULTS(!result);
	return result;
}

void time_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (!test_TimeInit()) p++;
	else f++;

	if (!test_TimeBasicOperations()) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // TIME_TESTS_HPP

