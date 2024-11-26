/**
 * author: brando
 * date: 11/22/24
 */

#ifndef INTERNAL_TREE_H
#define INTERNAL_TREE_H

#include "../tree.h"

typedef struct BFTreeNode {
	struct BFTreeNode * left;
	struct BFTreeNode * right;
	size_t height;
	BFTreeObject object;
} BFTreeNode;

BFTreeNode * BFTreeNodeCreate();
void BFTreeNodeRelease(BFTreeNode * node);

typedef struct _BFTree {
	BFTreeNode * root;
	size_t size;

	// a < b -> result < 0
	// a > b -> result > 0
	// a == b -> result == 0
	int (*compare)(BFTreeObject a, BFTreeObject b);

	/**
	 * used to to release every object once
	 * BFTreeRelease() is called
	 */
	void (*release)(BFTreeObject object);
} _BFTree;

/**
 * error: will nonzero if object couldn't be inserted.
 * 	one reason is there may be a duplicate
 *
 * returns node
 */
BFTreeNode * BFTreeNodeInsert(
	BFTreeNode * node,
	BFTreeObject object,
	int (*compare)(BFTreeObject a, BFTreeObject b),
	int * error
);

/**
 * returns node
 */
BFTreeNode * BFTreeNodeRemove(
	BFTreeNode * node,
	BFTreeObject object,
	int (*compare)(BFTreeObject a, BFTreeObject b)
);

/**
 * returns node
 */
bool BFTreeNodeSearch(
	BFTreeNode * node,
	BFTreeObject obj,
	int (*compare)(BFTreeObject a, BFTreeObject b)
);

#endif // INTERNAL_TREE_H

