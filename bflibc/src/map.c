/**
 * author: brando
 * date: 11/22/24
 */

#include "map.h"
#include "free.h"

int BFMapTreeCompare(BFTreeNodeObject aobj, BFTreeNodeObject bobj) {
	int a = *(int *) aobj;
	int b = *(int *) bobj;
	return a - b;
}

BFMap * BFMapCreate() {
	BFMap * res = (BFMap *) malloc(sizeof(BFMap));
	res->tree = BFTreeCreate();
	return res;
}

void BFMapSetCompare(BFMap * map, int (*compare)(BFMapKey a, BFMapKey b)) {
	if (!map) return;
	BFTreeSetCompare(map->tree, compare);
}

void BFMapRelease(BFMap * map) {
	BFTreeRelease(map->tree);
	BFFree(map);
}

int BFMapAdd(BFMap * map, BFMapKey key, BFMapValue value) {
	return 0;
}

void * BFMapGetValue(BFMap * map, BFMapKey key) {
	return 0;
}

int BFMapRemove(BFMap * map, BFMapKey key) {
	return 0;
}

