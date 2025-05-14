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

void BFSetRelease(BFSet _set) {
	_BFSet * set = (_BFSet *) _set;
	if (!set) return;

	BFTreeRelease(set->tree);
	BFFree(set);
}

