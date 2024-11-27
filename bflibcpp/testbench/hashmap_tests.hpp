/**
 * author: Brando
 * date: 11/27/24
 */

#ifndef HASH_MAP_TESTS_HPP
#define HASH_MAP_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include "hashmap.hpp"
#include "string.hpp"

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

BFTEST_UNIT_FUNC(test_hashMapInit, 2<<10,  {
	HashMap<String, int> map;
})

int BFTestHashMapCompareString(String a, String b) {
	return a.compareString(b);
}

unsigned long BFTestHashMapHashFunction(BFHashMapKey key) {
	return BFHashDjb2((unsigned char *) key);
}

BFTEST_UNIT_FUNC(test_hashMapInsert, 2<<10,  {
	HashMap<String, int> map;
	map.setCompare(BFTestHashMapCompareString);
	
	int mapsize = 2<<7;
	for (int i = 0; i < mapsize; i++) {
		String key("%d", i);
		int err = map.insert(key, i);
		BF_ASSERT(err == 0, "error inserting %d", err);
	}
})

BFTEST_UNIT_FUNC(test_hashMapGet, 2<<10,  {
	HashMap<String, int> map;
	map.setCompare(BFTestHashMapCompareString);

	// insert	
	int mapsize = 2<<7;
	for (int i = 0; i < mapsize; i++) {
		String key("%d", i);
		int err = map.insert(key, i);
		BF_ASSERT(err == 0, "error inserting %d", err);
	}

	// get
	for (int i = 0; i < mapsize; i++) {
		String key("%d", i);
		int err = 0;
		int value = map.getValueForKey(key, &err);
		BF_ASSERT(err == 0, "error getting: %d", err);
		BF_ASSERT(value == i, "%d != %d", value, i);
	}
})

BFTEST_UNIT_FUNC(test_hashMapRemove, 2<<10,  {
	if (BFTEST_UNIT_FUNC_ITR == 0) {
		BFRandInit(time(0));
	}
	HashMap<String, int> map;
	map.setCompare(BFTestHashMapCompareString);

	// insert	
	int mapsize = 2<<7;
	for (int i = 0; i < mapsize; i++) {
		String key("%d", i);
		int err = map.insert(key, i);
		BF_ASSERT(err == 0, "error inserting %d", err);
	}

	// remove
	int removeCount = 10;
	while (removeCount) {
		String key("%d", BFMathAbs(BFRand()) % mapsize);
		if (map.contains(key)) {
			int err = map.remove(key);
			BF_ASSERT(err == 0, "error removing for key=%s: %d", key.c_str(), err);
			removeCount--;
		}
	}
})

BFTEST_COVERAGE_FUNC(hashmap_tests, {
	BFTEST_LAUNCH(test_hashMapInit);
	BFTEST_LAUNCH(test_hashMapInsert);
	BFTEST_LAUNCH(test_hashMapGet);
	BFTEST_LAUNCH(test_hashMapRemove);

})

#endif // MAP_TESTS_HPP

