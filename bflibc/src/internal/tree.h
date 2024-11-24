/**
 * author: brando
 * date: 11/22/24
 */

#ifndef INTERNAL_TREE_H
#define INTERNAL_TREE_H

#include "../tree.h"

/**
 * returns node
 */
BFTreeNode * BFTreeNodeInsert(
	BFTreeNode * node,
	BFTreeNode * newNode,
	int (*compare)(BFTreeNodeObject a, BFTreeNodeObject b)
);

/**
 * returns node
 */
BFTreeNode * BFTreeNodeRemove(
	BFTreeNode * node,
	BFTreeNode * oldNode,
	int (*compare)(BFTreeNodeObject a, BFTreeNodeObject b)
);

/**
 * returns node
 */
BFTreeNode * BFTreeNodeSearch(
	BFTreeNode * node,
	BFTreeNodeObject obj,
	int (*compare)(BFTreeNodeObject a, BFTreeNodeObject b)
);

#endif // INTERNAL_TREE_H

