/**
 * author: Brando
 * date: 11/25/24
 */

#ifndef HASH_MAP_TESTS_H
#define HASH_MAP_TESTS_H

#include "clib_tests.h"
#include "hashmap.h"
#include "tree.h"
#include "hash.h"

unsigned long BFTestHashMapHashString(BFHashMapKey key) {
	return BFHashDjb2((unsigned char *) key);
}

BFTEST_UNIT_FUNC(test_hashMapInit, 2<<11, {
	BFHashMap map = BFHashMapCreate();
	BF_ASSERT(map, "map is null");
	BFHashMapRelease(map);
})

BFTEST_UNIT_FUNC(test_hashMapInsert, 2<<10, {
	BFHashMap map = BFHashMapCreate();
	BF_ASSERT(map, "map is null");
	BFHashMapSetHashFunction(map, BFTestHashMapHashString);

	int hmsize = 10;
	char keys[hmsize][64];
	int values[hmsize];
	for (int i = 0; i < hmsize; i++) {
		snprintf(keys[i], 64, "%d", i);
		values[i] = i;

		int err = BFHashMapInsert(map, (BFHashMapKey) keys[i], (BFHashMapValue) (intptr_t) values[i]);
		BF_ASSERT(err == 0, "error inserting %d", err);
	}

	BFHashMapRelease(map);
})

BFTEST_COVERAGE_FUNC(hashmap_tests, {
	BFTEST_LAUNCH(test_hashMapInit);
	BFTEST_LAUNCH(test_hashMapInsert);

})

#endif // HASH_MAP_TESTS_H

