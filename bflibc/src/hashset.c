/**
 * author: brando
 * date: 5/14/25
 */

#include "hashset.h"
#include "tree.h"
#include "free.h"

typedef struct _BFHashSet {
	BFHashMap tree;
} _BFHashSet;

BFHashSet BFHashSetCreate() {
	_BFHashSet * res = (_BFHashSet *) malloc(sizeof(_BFHashSet));
	if (!res) return NULL;

	res->tree = BFHashMapCreate();
	return (BFHashSet) res;
}

void BFHashSetSetCompare(BFHashSet _set, int (*compare)(BFHashSetValue a, BFHashSetValue b)) {
	_BFHashSet * set = (_BFHashSet *) _set;
	if (!set) return;

	BFHashMapSetCompare(set->tree, compare);
}

void BFHashSetSetRelease(BFHashSet _set, void (*release)(BFHashSetValue value)) {
	_BFHashSet * set = (_BFHashSet *) _set;
	if (!set) return;

	BFHashMapSetRelease(set->tree, release);
}

void BFHashSetRelease(BFHashSet _set) {
	_BFHashSet * set = (_BFHashSet *) _set;
	if (!set) return;

	BFHashMapRelease(set->tree);
	BFFree(set);
}

int BFHashSetInsert(BFHashSet _set, BFHashSetValue value) {
	_BFHashSet * set = (_BFHashSet *) _set;
	if (!set) return -1;

	return BFHashMapInsert(set->tree, value);
}

int BFHashSetRemove(BFHashSet _set, BFHashSetValue value) {
	_BFHashSet * set = (_BFHashSet *) _set;
	if (!set) return -1;

	return BFHashMapRemove(set->tree, value);
}

bool BFHashSetContains(BFHashSet _set, BFHashSetValue value) {
	_BFHashSet * set = (_BFHashSet *) _set;
	if (!set) return false;

	return BFHashMapContains(set->tree, value);
}

size_t BFHashSetGetSize(BFHashSet _set) {
	_BFHashSet * set = (_BFHashSet *) _set;
	if (!set) return 0;

	return BFHashMapSize(set->tree);
}

