/**
 * author: brando
 * date: 11/22/24
 */

#include "map.h"
#include "free.h"

typedef struct _BFMapKeyValuePair {
	BFMapKey key;
	BFMapValue value;

	// releases key and value
	void (*release)(BFMapKey key, BFMapValue value);
} _BFMapKeyValuePair;

BFMapKey BFMapKeyValuePairGetKey(BFMapKeyValuePair _pair) {
	_BFMapKeyValuePair * pair = (_BFMapKeyValuePair *) _pair;
	if (!pair) return NULL;
	return pair->key;
}

BFMapValue BFMapKeyValuePairGetValue(BFMapKeyValuePair _pair) {
	_BFMapKeyValuePair * pair = (_BFMapKeyValuePair *) _pair;
	if (!pair) return NULL;
	return pair->value;
}


typedef struct _BFMap {
	BFTree tree;
	
	// releases key and value
	void (*release)(BFMapKey key, BFMapValue value);
} _BFMap;

void _BFMapNodeRelease(BFTreeNodeObject object) {
	_BFMapKeyValuePair * pair = (_BFMapKeyValuePair *) object;
	if (pair->release) {
		pair->release(pair->key, pair->value);
	}
	BFFree(pair);
}

BFMap BFMapCreate() {
	_BFMap * res = (_BFMap *) malloc(sizeof(_BFMap));
	res->tree = BFTreeCreate();
	BFTreeSetRelease(res->tree, _BFMapNodeRelease);
	return res;
}

void BFMapSetCompare(BFMap _map, int (*compare)(BFMapKey a, BFMapKey b)) {
	_BFMap * map = (_BFMap *) _map;
	if (!map) return;
	BFTreeSetCompare(map->tree, compare);
}

void BFMapSetRelease(BFMap _map, void (*release)(BFMapKey key, BFMapValue value)) {
	_BFMap * map = (_BFMap *) _map;
	if (!map) return;
	map->release = release;
}

void BFMapRelease(BFMap _map) {
	_BFMap * map = (_BFMap *) _map;
	BFTreeRelease(map->tree);
	BFFree(map);
}

int BFMapInsert(BFMap _map, BFMapKey key, BFMapValue value) {
	_BFMap * map = (_BFMap *) _map;
	if (!map || !key || !value) {
		return -1;
	}
	_BFMapKeyValuePair * pair = (_BFMapKeyValuePair *) malloc(sizeof(_BFMapKeyValuePair));
	pair->key = key;
	pair->value = value;
	pair->release = map->release;
	int err = BFTreeInsert(map->tree, pair);
	if (err) {
		BFFree(pair);
	}
	return err;
}

void * BFMapGetValue(BFMap _map, BFMapKey key) {
	return 0;
}

int BFMapRemove(BFMap _map, BFMapKey key) {
	return 0;
}

