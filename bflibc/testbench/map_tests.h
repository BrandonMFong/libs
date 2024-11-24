/**
 * author: Brando
 * date: 11/22/24
 */

#ifndef MAP_TESTS_H
#define MAP_TESTS_H

#include "clib_tests.h"
#include "map.h"

BFTEST_UNIT_FUNC(test_mapinit, 2<<15, {
	BFMap * map = BFMapCreate();
	BF_ASSERT(map, "null map");
	BFMapRelease(map);
})

BFTEST_COVERAGE_FUNC(map_tests, {
	BFTEST_LAUNCH(test_mapinit);
})

#endif // MAP_TESTS_H

