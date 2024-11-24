/**
 * author: brando
 * date: 11/22/24
 */

#include "map.h"
#include "free.h"

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

int BFMapInsert(BFMap * map, BFMapKey key, BFMapValue value) {
	if (!map || !key || !value) {
		return -1;
	}
	BFMapKeyValuePair * pair = (BFMapKeyValuePair *) malloc(sizeof(BFMapKeyValuePair));
	pair->key = key;
	pair->value = value;
	return BFTreeInsert(map->tree, pair);
}

void * BFMapGetValue(BFMap * map, BFMapKey key) {
	return 0;
}

int BFMapRemove(BFMap * map, BFMapKey key) {
	return 0;
}

