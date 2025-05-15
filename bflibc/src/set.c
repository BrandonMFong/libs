/**
 * author: brando
 * date: 5/14/25
 */

#include "set.h"
#include "tree.h"
#include "free.h"

typedef struct _BFSet {
	BFTree tree;
} _BFSet;

BFSet BFSetCreate() {
	_BFSet * res = (_BFSet *) malloc(sizeof(_BFSet));
	if (!res) return NULL;

	res->tree = BFTreeCreate();
	return (BFSet) res;
}

void BFSetSetCompare(BFSet _set, int (*compare)(BFSetValue a, BFSetValue b)) {
	_BFSet * set = (_BFSet *) _set;
	if (!set) return;

	BFTreeSetCompare(set->tree, compare);
}

void BFSetSetRelease(BFSet _set, void (*release)(BFSetValue value)) {
	_BFSet * set = (_BFSet *) _set;
	if (!set) return;

	BFTreeSetRelease(set->tree, release);
}

void BFSetRelease(BFSet _set) {
	_BFSet * set = (_BFSet *) _set;
	if (!set) return;

	BFTreeRelease(set->tree);
	BFFree(set);
}

int BFSetInsert(BFSet _set, BFSetValue value) {
	_BFSet * set = (_BFSet *) _set;
	if (!set) return -1;

	return BFTreeInsert(set->tree, value);
}

int BFSetRemove(BFSet _set, BFSetValue value) {
	_BFSet * set = (_BFSet *) _set;
	if (!set) return -1;

	return BFTreeRemove(set->tree, value);
}

bool BFSetContains(BFSet _set, BFSetValue value) {
	_BFSet * set = (_BFSet *) _set;
	if (!set) return false;

	return BFTreeContains(set->tree, value);
}

size_t BFSetGetSize(BFSet _set) {
	_BFSet * set = (_BFSet *) _set;
	if (!set) return 0;

	return BFTreeSize(set->tree);
}

