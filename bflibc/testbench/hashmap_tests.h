/**
 * author: Brando
 * date: 11/25/24
 */

#ifndef HASH_MAP_TESTS_H
#define HASH_MAP_TESTS_H

#include "clib_tests.h"
#include "hashmap.h"
#include "tree.h"

BFTEST_UNIT_FUNC(test_hashMapInit, 2<<11, {
	BFHashMap map = BFHashMapCreate();
	BF_ASSERT(map, "map is null");
	BFHashMapRelease(map);
})

BFTEST_COVERAGE_FUNC(hashmap_tests, {
	BFTEST_LAUNCH(test_hashMapInit);
})

#endif // HASH_MAP_TESTS_H

