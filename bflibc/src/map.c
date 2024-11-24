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
	BFTreeSetCompare(res->tree, BFMapTreeCompare);
	return res;
}

void BFMapRelease(BFMap * map) {
	BFTreeRelease(map->tree);
	BFFree(map);
}

int BFMapAdd(BFMap * map, void * key, void * value) {
	return 0;
}

void * BFMapGetValue(BFMap * map, void * key) {
	return 0;
}

int BFMapRemove(BFMap * map, void * key) {
	return 0;
}

