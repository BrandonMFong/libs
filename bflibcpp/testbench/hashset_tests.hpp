/**
 * author: Brando
 * date: 5/27/25
 */

#ifndef HASH_SET_TESTS_HPP
#define HASH_SET_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include "hashset.hpp"
#include "string.hpp"

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

BFTEST_UNIT_FUNC(test_hashSetInit, 2<<10,  {
	HashSet<String> set;
})

BFTEST_UNIT_FUNC(test_hashSetInsert, 2<<10,  {
	HashSet<String> set;
	
	int setsize = 2<<7;
	for (int i = 0; i < setsize; i++) {
		String value("%d", i);
		int err = set.insert(value);
		BF_ASSERT(err == 0, "error inserting %d", err);
	}
})

BFTEST_UNIT_FUNC(test_hashSetRemove, 2<<10,  {
	if (BFTEST_UNIT_FUNC_ITR == 0) {
		BFRandInit(time(0));
	}
	HashSet<String> set;

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

BFTEST_UNIT_FUNC(test_hashSetWithAllocMem, 2<<10,  {
	if (BFTEST_UNIT_FUNC_ITR == 0) {
		BFRandInit(time(0));
	}
	HashSet<char *> set;

	// insert	
	int setsize = 2<<7;
	for (int i = 0; i < setsize; i++) {
		char * value = (char *) malloc(sizeof(char) * 32);
		snprintf(value, 32, "%d", i);
		int err = set.insert(value);
		BF_ASSERT(err == 0, "error inserting %d", err);
	}
})

BFTEST_COVERAGE_FUNC(hashset_tests, {
	BFTEST_LAUNCH(test_hashSetInit);
	BFTEST_LAUNCH(test_hashSetInsert);
	BFTEST_LAUNCH(test_hashSetRemove);
	BFTEST_LAUNCH(test_hashSetWithAllocMem);
})

#endif // HASH_SET_TESTS_HPP

