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
#include "rand.h"
#include <string.h>

unsigned long BFTestHashMapHashString(BFHashMapKey key) {
	return BFHashDjb2((unsigned char *) key);
}

int BFTestHashMapKeyCompare(BFHashMapKey akey, BFHashMapKey bkey) {
	const char * a = (const char *) akey;
	const char * b = (const char *) bkey;
	return strcmp(a, b);
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

	int hmsize = 2<<8;
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

BFTEST_UNIT_FUNC(test_hashMapInsertAndGet, 2<<10, {
	BFHashMap map = BFHashMapCreate();
	BF_ASSERT(map, "map is null");
	BFHashMapSetHashFunction(map, BFTestHashMapHashString);
	BFHashMapSetCompare(map, BFTestHashMapKeyCompare);

	int hmsize = 2<<8;
	char keys[hmsize][64];
	int values[hmsize];
	for (int i = 0; i < hmsize; i++) {
		snprintf(keys[i], 64, "%d", i);
		values[i] = i;

		int err = BFHashMapInsert(map, (BFHashMapKey) keys[i], (BFHashMapValue) (intptr_t) values[i]);
		BF_ASSERT(err == 0, "error inserting %d", err);
	}

	int randGetCount = 20;
	while (randGetCount--) {
		int i = BFMathAbs(BFRand()) % hmsize;
		const char * key = keys[i];
		BFHashMapValue value = BFHashMapGetValue(map, (BFHashMapKey) key);
		BF_ASSERT(values[i] == (intptr_t) value, "%d != %d", values[i], (intptr_t) value);
	}

	BFHashMapRelease(map);
})

BFTEST_COVERAGE_FUNC(hashmap_tests, {
	BFTEST_LAUNCH(test_hashMapInit);
	BFTEST_LAUNCH(test_hashMapInsert);
	BFTEST_LAUNCH(test_hashMapInsertAndGet);

})

#endif // HASH_MAP_TESTS_H

