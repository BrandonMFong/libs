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

int BFMapTreeCompare(BFTreeNodeObject aobj, BFTreeNodeObject bobj) {
	BFMapKeyValuePair * a = (BFMapKeyValuePair *) aobj;
	BFMapKeyValuePair * b = (BFMapKeyValuePair *) bobj;
	return strcmp(a->key, b->key);
}

BFTEST_UNIT_FUNC(test_mapinit, 2<<15, {
	BFMap * map = BFMapCreate();
	BF_ASSERT(map, "null map");
	BFMapSetCompare(map, BFMapTreeCompare);
	BFMapRelease(map);
})

BFTEST_UNIT_FUNC(test_mapInsert, 1, {
	BFMap * map = BFMapCreate();
	BF_ASSERT(map, "null map");
	BFMapSetCompare(map, BFMapTreeCompare);

	// making map<char*, int>[mapsize]
	int mapsize = 10;
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

	for (int i = 0; i < mapsize; i++) {
		BFFree(keys[i]);
		BFFree(values[i]);
	}

	BFMapRelease(map);
})

BFTEST_COVERAGE_FUNC(map_tests, {
	BFTEST_LAUNCH(test_mapinit);
	//BFTEST_LAUNCH(test_mapInsert);

})

#endif // MAP_TESTS_H

