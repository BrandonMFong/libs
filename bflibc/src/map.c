/**
 * author: brando
 * date: 11/22/24
 */

#include "map.h"
#include "free.h"
#include "internal/tree.h"

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

void _BFMapNodeRelease(BFTreeObject object) {
	_BFMapKeyValuePair * pair = (_BFMapKeyValuePair *) object;
	if (pair->release) {
		pair->release(pair->key, pair->value);
	}
	BFFree(pair);
}

BFMap BFMapCreate() {
	_BFMap * res = (_BFMap *) malloc(sizeof(_BFMap));
	res->tree = BFTreeCreate();
	res->release = NULL;
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

BFMapKeyValuePair _BFMapGetValueFromTree(
	BFTreeNode * node,
	BFMapKeyValuePair pair,
	int (*compare)(BFMapKey a, BFMapKey b)
) {
	if (!node) {
		return NULL;
	}

	int comp = compare(pair, node->object);
	if (comp == 0) {
		return (BFMapKeyValuePair) node->object;
	} else if (comp < 0) {
		return _BFMapGetValueFromTree(node->left, pair, compare);
	} else {
		return _BFMapGetValueFromTree(node->right, pair, compare);
	}
}

BFMapValue BFMapGetValue(BFMap _map, BFMapKey key) {
	_BFMap * map = (_BFMap *) _map;
	if (!map || !key) {
		return NULL;
	}
	
	_BFTree * tree = (_BFTree *) map->tree;
	if (!tree) {
		return NULL;
	}

	// temporarily using this structure so it can 
	// pass through the compare callback
	_BFMapKeyValuePair tmp;
	tmp.key = key;

	BFMapKeyValuePair pair = _BFMapGetValueFromTree(tree->root, &tmp, tree->compare);
	if (!pair) {
		return NULL;
	}

	return BFMapKeyValuePairGetValue(pair);
}

int BFMapRemove(BFMap _map, BFMapKey key) {
	_BFMap * map = (_BFMap *) _map;
	if (!map || !key) {
		return -1;
	}
	
	_BFTree * tree = (_BFTree *) map->tree;
	if (!tree) {
		return -1;
	}

	// temporarily using this structure so it can 
	// pass through the compare callback
	_BFMapKeyValuePair tmp;
	tmp.key = key;

	// is there a better way than traversing through the tree
	// to find the object we want to delete?
	BFMapKeyValuePair pair = _BFMapGetValueFromTree(tree->root, &tmp, tree->compare);
	if (!pair) {
		return -1;
	}

	return BFTreeRemove(tree, pair);
}

