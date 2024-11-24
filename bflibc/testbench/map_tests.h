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

int BFTestMapTreeCompare(BFTreeNodeObject aobj, BFTreeNodeObject bobj) {
	BFMapKey a = BFMapKeyValuePairGetKey((BFMapKeyValuePair) aobj);
	BFMapKey b = BFMapKeyValuePairGetKey((BFMapKeyValuePair) bobj);
	return strcmp(a, b);
}

void BFTestMapTreeRelease(BFMapKey key, BFMapValue value) {
	BFFree(key);
	BFFree(value);
}

BFTEST_UNIT_FUNC(test_mapinit, 2<<10, {
	BFMap * map = BFMapCreate();
	BF_ASSERT(map, "null map");
	BFMapSetCompare(map, BFTestMapTreeCompare);
	BFMapRelease(map);
})

BFTEST_UNIT_FUNC(test_mapInsert, 2<<10, {
	BFMap * map = BFMapCreate();
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
	BFMap * map = BFMapCreate();
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
		int index = abs(BFRand()) % mapsize;
		BFMapValue value = BFMapGetValue(map, keys[index]);
		BF_ASSERT(value, "value is null");
		BF_ASSERT(*(int*) value == *values[index], "%d != %d", *(int*) value, *values[index]);
	}

	BFMapRelease(map);
})

BFTEST_UNIT_FUNC(test_mapRemove, 1, {
	BFMap * map = BFMapCreate();
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
		int index = abs(BFRand()) % mapsize;

		if (keys[index]) {
			int err = BFMapRemove(map, keys[index]);
			BF_ASSERT(err == 0, "removal error for key=%s", keys[index]);
			keys[index] = NULL;
		}
	}

	BFMapRelease(map);
})

BFTEST_COVERAGE_FUNC(map_tests, {
	BFTEST_LAUNCH(test_mapinit);
	BFTEST_LAUNCH(test_mapInsert);
	BFTEST_LAUNCH(test_mapGet);
	BFTEST_LAUNCH(test_mapRemove);

})

#endif // MAP_TESTS_H

