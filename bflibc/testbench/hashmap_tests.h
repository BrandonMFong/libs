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
	BF_ASSERT(BFHashMapGetSize(map) == hmsize, "size %ld != %ld", BFHashMapGetSize(map), hmsize);

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
	BF_ASSERT(BFHashMapGetSize(map) == hmsize, "size %ld != %ld", BFHashMapGetSize(map), hmsize);

	int randGetCount = 20;
	while (randGetCount--) {
		int i = BFMathAbs(BFRand()) % hmsize;
		const char * key = keys[i];
		BFHashMapValue value = BFHashMapGetValue(map, (BFHashMapKey) key, NULL);
		BF_ASSERT(values[i] == (intptr_t) value, "%d != %d", values[i], (intptr_t) value);
	}

	BFHashMapRelease(map);
})

BFTEST_UNIT_FUNC(test_hashMapRemove, 2<<10, {
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
	BF_ASSERT(BFHashMapGetSize(map) == hmsize, "size %ld != %ld", BFHashMapGetSize(map), hmsize);

	int randGetCount = 20;
	while (randGetCount--) {
		int i = BFMathAbs(BFRand()) % hmsize;
		const char * key = keys[i];
		if (BFHashMapContains(map, (BFHashMapKey) key)) {
			int err = BFHashMapRemove(map, (BFHashMapKey) key);
			BF_ASSERT(err == 0, "error removing %d", err);
		}
	}

	BFHashMapRelease(map);
})

BFTEST_UNIT_FUNC(test_hashMapGetNonExistentKeyValue, 2<<10, {
	BFHashMap map = BFHashMapCreate();
	BF_ASSERT(map, "map is null");
	BFHashMapSetHashFunction(map, BFTestHashMapHashString);
	BFHashMapSetCompare(map, BFTestHashMapKeyCompare);

	int hmsize = 2<<8;
	char keys[hmsize][64];
	int values[hmsize];
	for (int i = 0; i < hmsize; i += 2) {
		snprintf(keys[i], 64, "%d", i);
		values[i] = i;

		int err = BFHashMapInsert(map, (BFHashMapKey) keys[i], (BFHashMapValue) (intptr_t) values[i]);
		BF_ASSERT(err == 0, "error inserting %d", err);
	}

	BF_ASSERT(BFHashMapGetSize(map) == (hmsize/2), "size %ld != %ld", BFHashMapGetSize(map), (hmsize/2));

	char key[64];	
	for (int i = 1; i < hmsize; i += 2) {
		snprintf(key, 64, "%d", i);

		int err = 0;
		BFHashMapGetValue(map, (BFHashMapKey) key, &err);
		BF_ASSERT(err != 0, "key=%s should not be in the hash map", key);
	}

	BFHashMapRelease(map);
})

BFTEST_COVERAGE_FUNC(hashmap_tests, {
	BFTEST_LAUNCH(test_hashMapInit);
	BFTEST_LAUNCH(test_hashMapInsert);
	BFTEST_LAUNCH(test_hashMapInsertAndGet);
	BFTEST_LAUNCH(test_hashMapRemove);
	BFTEST_LAUNCH(test_hashMapGetNonExistentKeyValue);

})

#endif // HASH_MAP_TESTS_H

