/**
 * author: brando
 * date: 11/22/24
 */

#include "map.h"
#include "free.h"

void _BFMapNodeRelease(BFTreeNodeObject object) {
	BFMapKeyValuePair * pair = (BFMapKeyValuePair *) object;
	if (pair->release) {
		pair->release(pair->key, pair->value);
	}
	BFFree(pair);
}

BFMap * BFMapCreate() {
	BFMap * res = (BFMap *) malloc(sizeof(BFMap));
	res->tree = BFTreeCreate();
	BFTreeSetRelease(res->tree, _BFMapNodeRelease);
	return res;
}

void BFMapSetCompare(BFMap * map, int (*compare)(BFMapKey a, BFMapKey b)) {
	if (!map) return;
	BFTreeSetCompare(map->tree, compare);
}

void BFMapSetRelease(BFMap * map, void (*release)(BFMapKey key, BFMapValue value)) {
	if (!map) return;
	map->release = release;
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
	pair->release = map->release;
	int err = BFTreeInsert(map->tree, pair);
	if (err) {
		BFFree(pair);
	}
	return err;
}

void * BFMapGetValue(BFMap * map, BFMapKey key) {
	return 0;
}

int BFMapRemove(BFMap * map, BFMapKey key) {
	return 0;
}

