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
	BFTreeNodeObject object;
} BFTreeNode;

BFTreeNode * BFTreeNodeCreate();
void BFTreeNodeRelease(BFTreeNode * node);

typedef struct _BFTree {
	BFTreeNode * root;
	size_t size;

	// a < b -> result < 0
	// a > b -> result > 0
	// a == b -> result == 0
	int (*compare)(BFTreeNodeObject a, BFTreeNodeObject b);

	/**
	 * used to to release every object once
	 * BFTreeRelease() is called
	 */
	void (*release)(BFTreeNodeObject object);
} _BFTree;

/**
 * returns node
 */
BFTreeNode * BFTreeNodeInsert(
	BFTreeNode * node,
	BFTreeNodeObject object,
	int (*compare)(BFTreeNodeObject a, BFTreeNodeObject b)
);

/**
 * returns node
 */
BFTreeNode * BFTreeNodeRemove(
	BFTreeNode * node,
	BFTreeNodeObject object,
	int (*compare)(BFTreeNodeObject a, BFTreeNodeObject b)
);

/**
 * returns node
 */
bool BFTreeNodeSearch(
	BFTreeNode * node,
	BFTreeNodeObject obj,
	int (*compare)(BFTreeNodeObject a, BFTreeNodeObject b)
);

#endif // INTERNAL_TREE_H

