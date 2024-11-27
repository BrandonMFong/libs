/**
 * author: brando
 * date: 11/22/24
 */

#include "map.h"
#include "free.h"
#include "internal/map.h"
#include "tree.h"

void _BFMapNodeRelease(BFTreeObject object) {
	_BFMapKeyValuePair * pair = (_BFMapKeyValuePair *) object;
	if (pair->release) {
		pair->release(pair->key, pair->value);
	}
	BFFree(pair);
}

int _BFMapNodeCompare(BFTreeObject aobj, BFTreeObject bobj) {
	_BFMapKeyValuePair * apair = (_BFMapKeyValuePair *) aobj;
	_BFMapKeyValuePair * bpair = (_BFMapKeyValuePair *) bobj;
	return apair->compare(apair->key, bpair->key);
}

BFMap BFMapCreate() {
	_BFMap * res = (_BFMap *) malloc(sizeof(_BFMap));
	res->tree = BFTreeCreate();
	res->release = NULL;
	BFTreeSetCompare(res->tree, _BFMapNodeCompare);
	BFTreeSetRelease(res->tree, _BFMapNodeRelease);
	return res;
}

void BFMapSetCompare(BFMap _map, int (*compare)(BFMapKey a, BFMapKey b)) {
	_BFMap * map = (_BFMap *) _map;
	if (!map) return;
	map->compare = compare;
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

size_t BFMapGetSize(BFMap _map) {
	_BFMap * map = (_BFMap *) _map;
	if (!map) return 0;
	return BFTreeSize(map->tree);
}

int BFMapInsert(BFMap _map, BFMapKey key, BFMapValue value) {
	_BFMap * map = (_BFMap *) _map;
	if (!map) {
		return -1;
	}
	_BFMapKeyValuePair * pair = (_BFMapKeyValuePair *) malloc(sizeof(_BFMapKeyValuePair));
	pair->key = key;
	pair->value = value;
	pair->release = map->release;
	pair->compare = map->compare;
	int err = BFTreeInsert(map->tree, pair);
	if (err) {
		BFFree(pair);
	}
	return err;
}

_BFMapKeyValuePair * _BFMapGetValueFromTree(
	BFTreeNode * node,
	_BFMapKeyValuePair * inpair,
	int (*compare)(BFMapKey a, BFMapKey b)
) {
	if (!node || !inpair) {
		return NULL;
	}

	_BFMapKeyValuePair * pair = BFTreeNodeGetObject(node);
	if (!pair) {
		return NULL;
	}

	BFMapKey akey = inpair->key;
	BFMapKey bkey = pair->key;
	if (!akey || !bkey) {
		return NULL;
	}

	int comp = compare(akey, bkey);
	if (comp == 0) {
		return (_BFMapKeyValuePair *) BFTreeNodeGetObject(node);
	} else if (comp < 0) {
		return _BFMapGetValueFromTree(BFTreeNodeGetLeft(node), inpair, compare);
	} else {
		return _BFMapGetValueFromTree(BFTreeNodeGetRight(node), inpair, compare);
	}
}

BFMapValue BFMapGetValue(BFMap _map, BFMapKey key, int * error) {
	_BFMap * map = (_BFMap *) _map;
	if (!map || !map->tree) {
		if (error) *error = -1;
		return NULL;
	}

	// temporarily using this structure so it can 
	// pass through the compare callback
	_BFMapKeyValuePair tmp;
	tmp.key = key;

	_BFMapKeyValuePair * pair = _BFMapGetValueFromTree(
		BFTreeGetRoot(map->tree),
		&tmp,
		map->compare
	);
	if (!pair) {
		if (error) *error = -1;
		return NULL;
	}

	return pair->value;
}

int BFMapRemove(BFMap _map, BFMapKey key) {
	_BFMap * map = (_BFMap *) _map;
	if (!map || !map->tree) {
		return -1;
	}

	// temporarily using this structure so it can 
	// pass through the compare callback
	_BFMapKeyValuePair tmp;
	tmp.key = key;

	// is there a better way than traversing through the tree
	// to find the object we want to delete?
	_BFMapKeyValuePair * pair = _BFMapGetValueFromTree(
		BFTreeGetRoot(map->tree),
		&tmp,
		map->compare
	);
	if (!pair) {
		return -1;
	}

	return BFTreeRemove(map->tree, pair);
}

bool BFMapContains(BFMap _map, BFMapKey key) {
	_BFMap * map = (_BFMap *) _map;
	if (!map || !map->tree) {
		return -1;
	}

	// temporarily using this structure so it can 
	// pass through the compare callback
	_BFMapKeyValuePair tmp;
	tmp.key = key;

	// is there a better way than traversing through the tree
	// to find the object we want to delete?
	_BFMapKeyValuePair * pair = _BFMapGetValueFromTree(
		BFTreeGetRoot(map->tree),
		&tmp,
		map->compare
	);

	return pair != NULL;
}

