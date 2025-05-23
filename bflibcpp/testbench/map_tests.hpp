/**
 * author: Brando
 * date: 11/26/24
 */

#ifndef MAP_TESTS_HPP
#define MAP_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include "map.hpp"

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

BFTEST_UNIT_FUNC(test_mapInit, 2<<10,  {
	Map<String, int> map;
})

int BFTestMapCompareString(String & a, String & b) {
	return a.compareString(b);
}

BFTEST_UNIT_FUNC(test_mapInsert, 2<<10,  {
	Map<String, int> map;
	//map.setCompare(BFTestMapCompareString);
	
	int mapsize = 2<<7;
	for (int i = 0; i < mapsize; i++) {
		String key("%d", i);
		int err = map.insert(key, i);
		BF_ASSERT(err == 0, "error inserting %d", err);
	}
})

BFTEST_UNIT_FUNC(test_mapGet, 2<<10,  {
	Map<String, int> map;
	//map.setCompare(BFTestMapCompareString);

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

BFTEST_UNIT_FUNC(test_mapRemove, 2<<10,  {
	if (BFTEST_UNIT_FUNC_ITR == 0) {
		BFRandInit(time(0));
	}
	Map<String, int> map;
	//map.setCompare(BFTestMapCompareString);

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

int BFTestMapCompareCString(char * &a, char * &b) {
	return strcmp(a, b);
}

void BFTestMapReleaseKeyCString(char * obj) {
	BFFree(obj);
}

void BFTestMapReleaseValueInteger(int * obj) {
	BFFree(obj);
}

BFTEST_UNIT_FUNC(test_mapWithAllocMem, 2<<10,  {
	if (BFTEST_UNIT_FUNC_ITR == 0) {
		BFRandInit(time(0));
	}
	Map<char *, int *> map;
	//map.setCompare(BFTestMapCompareCString);
	map.setRelease(BFTestMapReleaseKeyCString, BFTestMapReleaseValueInteger);

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

BFTEST_UNIT_FUNC(test_mapGetWithSubscript, 2<<10,  {
	Map<String, int> map;
	//map.setCompare(BFTestMapCompareString);

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

BFTEST_COVERAGE_FUNC(map_tests, {
	BFTEST_LAUNCH(test_mapInit);
	BFTEST_LAUNCH(test_mapInsert);
	BFTEST_LAUNCH(test_mapGet);
	BFTEST_LAUNCH(test_mapRemove);
	BFTEST_LAUNCH(test_mapWithAllocMem);
	BFTEST_LAUNCH(test_mapGetWithSubscript);

})

#endif // MAP_TESTS_HPP

