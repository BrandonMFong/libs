/**
 * author: Brando
 * date: 11/22/24
 */

#ifndef MAP_TESTS_H
#define MAP_TESTS_H

#include "clib_tests.h"
#include "map.h"
#include "free.h"
#include <string.h>

int BFTestMapTreeCompare(BFMapKey a, BFMapKey b) {
	return strcmp(a, b);
}

void BFTestMapTreeRelease(BFMapKey key, BFMapValue value) {
	BFFree(key);
	BFFree(value);
}

BFTEST_UNIT_FUNC(test_mapinit, 2<<10, {
	BFMap map = BFMapCreate();
	BF_ASSERT(map, "null map");
	BFMapSetCompare(map, BFTestMapTreeCompare);
	BFMapRelease(map);
})

BFTEST_UNIT_FUNC(test_mapInsert, 2<<10, {
	BFMap map = BFMapCreate();
	BF_ASSERT(map, "null map");
	BFMapSetCompare(map, BFTestMapTreeCompare);
	BFMapSetRelease(map, BFTestMapTreeRelease);

	// making map<char*, int>[mapsize]
	int mapsize = 2<<6;
	char * keys[mapsize];
	int * values[mapsize];
	for (int i = 0; i < mapsize; i++) {
		keys[i] = (char *) malloc(sizeof(char) * 32);
		snprintf(keys[i], 32, "%d", i);
		values[i] = (int *) malloc(sizeof(int));
		*values[i] = i;

		int err = BFMapInsert(map, keys[i], values[i]);
		BF_ASSERT(err == 0, "insert error %d", err);
	}

	BFMapRelease(map);
})

BFTEST_UNIT_FUNC(test_mapGet, 2<<10, {
	BFMap map = BFMapCreate();
	BF_ASSERT(map, "null map");
	BFMapSetCompare(map, BFTestMapTreeCompare);
	BFMapSetRelease(map, BFTestMapTreeRelease);

	// making map<char*, int>[mapsize]
	int mapsize = 2<<6;
	char * keys[mapsize];
	int * values[mapsize];
	for (int i = 0; i < mapsize; i++) {
		keys[i] = (char *) malloc(sizeof(char) * 32);
		snprintf(keys[i], 32, "%d", i);
		values[i] = (int *) malloc(sizeof(int));
		*values[i] = i;

		int err = BFMapInsert(map, keys[i], values[i]);
		BF_ASSERT(err == 0, "insert error %d", err);
	}
	
	int randNumSearch = 20;
	while (randNumSearch--) {
		int index = BFMathAbs(BFRand()) % mapsize;
		BFMapValue value = BFMapGetValue(map, keys[index], NULL);
		BF_ASSERT(value, "value is null");
		BF_ASSERT(*(int*) value == *values[index], "%d != %d", *(int*) value, *values[index]);
	}

	BFMapRelease(map);
})

BFTEST_UNIT_FUNC(test_mapRemove, 2<<10, {
	BFMap map = BFMapCreate();
	BF_ASSERT(map, "null map");
	BFMapSetCompare(map, BFTestMapTreeCompare);
	BFMapSetRelease(map, BFTestMapTreeRelease);

	// making map<char*, int>[mapsize]
	int mapsize = 2<<6;
	char * keys[mapsize];
	int * values[mapsize];
	for (int i = 0; i < mapsize; i++) {
		keys[i] = (char *) malloc(sizeof(char) * 32);
		snprintf(keys[i], 32, "%d", i);
		values[i] = (int *) malloc(sizeof(int));
		*values[i] = i;

		int err = BFMapInsert(map, keys[i], values[i]);
		BF_ASSERT(err == 0, "insert error %d", err);
	}
	
	int randNumRemove = 20;
	while (randNumRemove--) {
		int index = BFMathAbs(BFRand()) % mapsize;

		if (keys[index]) {
			int err = BFMapRemove(map, keys[index]);
			BF_ASSERT(err == 0, "removal error for key=%s", keys[index]);
			keys[index] = NULL; // mark as already removed
		}
	}

	BFMapRelease(map);
})

int BFTestMapTreeCompareInteger(BFMapKey aobj, BFMapKey bobj) {
	int a = (intptr_t) aobj;
	int b = (intptr_t) bobj;
	return a - b;
}

BFTEST_UNIT_FUNC(test_mapKeyValueNoPointers, 2<<10, {
	BFMap map = BFMapCreate();
	BF_ASSERT(map, "null map");
	BFMapSetCompare(map, BFTestMapTreeCompareInteger);

	// making map<char*, int>[mapsize]
	int mapsize = 2<<6;
	int keys[mapsize];
	int values[mapsize];
	for (int i = 0; i < mapsize; i++) {
		keys[i] = i+1;
		values[i] = i+1;

		int err = BFMapInsert(map, (BFMapKey) (intptr_t) keys[i], (BFMapValue) (intptr_t) values[i]);
		BF_ASSERT(err == 0, "insert error %d", err);
	}

	int randNumRemove = 20;
	while (randNumRemove--) {
		int index = BFMathAbs(BFRand()) % mapsize;
		if (keys[index] > 0) {
			int err = BFMapRemove(map, (BFMapKey) (intptr_t) keys[index]);
			BF_ASSERT(err == 0, "removal error for key=%d", keys[index]);
			keys[index] = 0; // mark as already removed
		}
	}

	BFMapRelease(map);
})

BFTEST_UNIT_FUNC(test_mapGetNonexistentKeyValue, 2<<10, {
	BFMap map = BFMapCreate();
	BF_ASSERT(map, "null map");
	BFMapSetCompare(map, BFTestMapTreeCompareInteger);

	// making map<char*, int>[mapsize]
	int mapsize = 2<<6;
	int keys[mapsize];
	int values[mapsize];
	for (int i = 0; i < mapsize; i += 2) {
		keys[i] = i;
		values[i] = i;

		int err = BFMapInsert(map, (BFMapKey) (intptr_t) keys[i], (BFMapValue) (intptr_t) values[i]);
		BF_ASSERT(err == 0, "insert error %d", err);
	}

	BF_ASSERT(BFMapGetSize(map) == mapsize / 2, "size is not correct, actual=%ld expect=%ld", BFMapGetSize(map), mapsize / 2);

	for (int i = 1; i < mapsize; i += 2) {
		int err = 0;
		BFMapGetValue(map, (BFMapKey) (intptr_t) i, &err);
		BF_ASSERT(err != 0, "", err);
	}

	BFMapRelease(map);
})

BFTEST_UNIT_FUNC(test_mapContains, 2<<10, {
	BFMap map = BFMapCreate();
	BF_ASSERT(map, "null map");
	BFMapSetCompare(map, BFTestMapTreeCompareInteger);

	// making map<char*, int>[mapsize]
	int mapsize = 2<<6;
	int keys[mapsize];
	int values[mapsize];
	for (int i = 0; i < mapsize; i += 2) {
		keys[i] = i;
		values[i] = i;

		int err = BFMapInsert(map, (BFMapKey) (intptr_t) keys[i], (BFMapValue) (intptr_t) values[i]);
		BF_ASSERT(err == 0, "insert error %d", err);
	}

	BF_ASSERT(BFMapGetSize(map) == mapsize / 2, "size is not correct, actual=%ld expect=%ld", BFMapGetSize(map), mapsize / 2);

	for (int i = 0; i < mapsize; i++) {
		BF_ASSERT(
			BFMapContains(map, (BFMapKey) (intptr_t) keys[i]) == (i % 2 == 0 ? true : false),
			"BFMapContains(key=%d) should be %s",
			keys[i], (i % 2 == 0 ? "true" : "false"));
	}

	BFMapRelease(map);
})

BFTEST_COVERAGE_FUNC(map_tests, {
		/*
	BFTEST_LAUNCH(test_mapinit);
	BFTEST_LAUNCH(test_mapInsert);
	BFTEST_LAUNCH(test_mapGet);
	BFTEST_LAUNCH(test_mapRemove);
	BFTEST_LAUNCH(test_mapKeyValueNoPointers);
	BFTEST_LAUNCH(test_mapGetNonexistentKeyValue);
	*/
	BFTEST_LAUNCH(test_mapContains);

})

#endif // MAP_TESTS_H

