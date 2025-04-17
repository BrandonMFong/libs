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

int BFTestHashMapCompareString(String & a, String & b) {
	return a.compareString(b);
}

unsigned long BFTestHashMapHashFunctionString(String & key) {
	return BFHashDjb2((unsigned char *) key.c_str());
}

BFTEST_UNIT_FUNC(test_hashMapInsert, 2<<10,  {
	HashMap<String, int> map;
	map.setCompare(BFTestHashMapCompareString);
	map.setHash(BFTestHashMapHashFunctionString);
	
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
	map.setHash(BFTestHashMapHashFunctionString);

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
		try {
			int value = map.getValueForKey(key);
			BF_ASSERT(value == i, "%d != %d", value, i);
		} catch (Exception & e) {
			BF_ASSERT(false, "%s", e.what());
		}
	}
})

BFTEST_UNIT_FUNC(test_hashMapRemove, 2<<10,  {
	if (BFTEST_UNIT_FUNC_ITR == 0) {
		BFRandInit(time(0));
	}
	HashMap<String, int> map;
	map.setCompare(BFTestHashMapCompareString);
	map.setHash(BFTestHashMapHashFunctionString);

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

int BFTestHashMapCompareCString(char * &a, char * &b) {
	return strcmp(a, b);
}

void BFTestHashMapReleaseKeyCString(char * obj) {
	BFFree(obj);
}

void BFTestHashMapReleaseValueInteger(int * obj) {
	BFFree(obj);
}

unsigned long BFTestHashMapHashFunctionCString(char * &key) {
	return BFHashDjb2((unsigned char *) key);
}

BFTEST_UNIT_FUNC(test_hashMapWithAllocMem, 2<<10,  {
	if (BFTEST_UNIT_FUNC_ITR == 0) {
		BFRandInit(time(0));
	}
	HashMap<char *, int *> map;
	map.setCompare(BFTestHashMapCompareCString);
	map.setHash(BFTestHashMapHashFunctionCString);
	map.setRelease(BFTestHashMapReleaseKeyCString, BFTestHashMapReleaseValueInteger);

	// insert	
	int mapsize = 2<<7;
	for (int i = 0; i < mapsize; i++) {
		char * key = (char *) malloc(sizeof(char) * 32);
		snprintf(key, 32, "%d", i);
		int * value = (int *) malloc(sizeof(int));
		*value = i;
		int err = map.insert(key, value);
		BF_ASSERT(err == 0, "error inserting %d", err);
	}
})

BFTEST_UNIT_FUNC(test_hashMapGetterWithSubscript, 2<<10,  {
	HashMap<String, int> map;
	map.setCompare(BFTestHashMapCompareString);
	map.setHash(BFTestHashMapHashFunctionString);

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
		try {
			int value = map[key];
			BF_ASSERT(value == i, "%d != %d", value, i);
		} catch (Exception & e) {
			BF_ASSERT(false, "%s", e.what());
		}
	}

})

BFTEST_UNIT_FUNC(test_getterWithDefaultCompare, 2<<10, {
	HashMap<String, String> map;
	map.insert("one", "1");
	map.insert("two", "2");
	map.insert("three", "3");
	BF_ASSERT(map["one"] == "1");
	BF_ASSERT(map["two"] == "2");
	BF_ASSERT(map["three"] == "3");
})

BFTEST_COVERAGE_FUNC(hashmap_tests, {
	BFTEST_LAUNCH(test_hashMapInit);
	BFTEST_LAUNCH(test_hashMapInsert);
	BFTEST_LAUNCH(test_hashMapGet);
	BFTEST_LAUNCH(test_hashMapRemove);
	BFTEST_LAUNCH(test_hashMapWithAllocMem);
	BFTEST_LAUNCH(test_hashMapGetterWithSubscript);
	BFTEST_LAUNCH(test_getterWithDefaultCompare);
})

#endif // MAP_TESTS_HPP

