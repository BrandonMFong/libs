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
	map.setCompare(BFTestMapCompareString);
	
	int mapsize = 2<<7;
	for (int i = 0; i < mapsize; i++) {
		String key("%d", i);
		int err = map.insert(key, i);
		BF_ASSERT(err == 0, "error inserting %d", err);
	}
})

BFTEST_COVERAGE_FUNC(map_tests, {
	BFTEST_LAUNCH(test_mapInit);
	BFTEST_LAUNCH(test_mapInsert);

})

#endif // MAP_TESTS_HPP

