/**
 * author: Brando
 * date: 5/27/25
 */

#ifndef SET_TESTS_HPP
#define SET_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include "set.hpp"

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

BFTEST_UNIT_FUNC(test_setInit, 2<<10,  {
	Set<String> set;
})

BFTEST_UNIT_FUNC(test_setInsert, 2<<10,  {
	Set<String> set;
	
	int setsize = 2<<7;
	for (int i = 0; i < setsize; i++) {
		String value("%d", i);
		int err = set.insert(value);
		BF_ASSERT(err == 0, "error inserting %d", err);
	}
})

BFTEST_UNIT_FUNC(test_setContains, 2<<10,  {
	Set<String> set;

	// insert	
	int setsize = 2<<7;
	for (int i = 0; i < setsize; i++) {
		String value("%d", i);
		int err = set.insert(value);
		BF_ASSERT(err == 0, "error inserting %d", err);
	}

	// get
	for (int i = 0; i < setsize; i++) {
		String value("%d", i);
		try {
			BF_ASSERT(set.contains(value), "!contains(%s)", value.cString());
		} catch (Exception & e) {
			BF_ASSERT(false, "%s", e.what());
		}
	}
})

BFTEST_UNIT_FUNC(test_setRemove, 2<<10,  {
	if (BFTEST_UNIT_FUNC_ITR == 0) {
		BFRandInit(time(0));
	}
	Set<String> set;

	// insert	
	int setsize = 2<<7;
	for (int i = 0; i < setsize; i++) {
		String value("%d", i);
		int err = set.insert(value);
		BF_ASSERT(err == 0, "error inserting %d", err);
	}

	// remove
	int removeCount = 10;
	while (removeCount) {
		String value("%d", BFMathAbs(BFRand()) % setsize);
		if (set.contains(value)) {
			int err = set.remove(value);
			BF_ASSERT(err == 0, "error removing for key=%s: %d", value.c_str(), err);
			removeCount--;
		}
	}
})

BFTEST_UNIT_FUNC(test_setWithAllocMem, 2<<10,  {
	if (BFTEST_UNIT_FUNC_ITR == 0) {
		BFRandInit(time(0));
	}
	Set<char *> set;

	// insert	
	int setsize = 2<<7;
	for (int i = 0; i < setsize; i++) {
		char * value = (char *) malloc(sizeof(char) * 32);
		snprintf(value, 32, "%d", i);
		int err = set.insert(value);
		BF_ASSERT(err == 0, "error inserting %d", err);
	}
})

BFTEST_COVERAGE_FUNC(set_tests, {
	BFTEST_LAUNCH(test_setInit);
	BFTEST_LAUNCH(test_setInsert);
	BFTEST_LAUNCH(test_setContains);
	BFTEST_LAUNCH(test_setRemove);
	BFTEST_LAUNCH(test_setWithAllocMem);
})

#endif // SET_TESTS_HPP

