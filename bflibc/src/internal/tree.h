/**
 * author: brando
 * date: 11/22/24
 */

#ifndef INTERNAL_TREE_H
#define INTERNAL_TREE_H

#include "../tree.h"

typedef struct _BFTreeNode {
	struct _BFTreeNode * left;
	struct _BFTreeNode * right;
	size_t height;
	BFTreeObject object;
} _BFTreeNode;

_BFTreeNode * _BFTreeNodeCreate();
void _BFTreeNodeRelease(
	_BFTreeNode * node,
	void (*release)(BFTreeObject object)
);

typedef struct _BFTree {
	_BFTreeNode * root;
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

	/**
	 * |x|x|x|x|x|x|x|<allow duplicates>|
	 *
	 * <allow duplicates>: default value == 0. 0: not allow. 1: allow
	 */
	unsigned char flags;
} _BFTree;

#define _BFTREE_FLAG_ALLOW_DUPLICATES 0

/**
 * error: will nonzero if object couldn't be inserted.
 * 	one reason is there may be a duplicate
 *
 * flags: _BFTree::flags
 *
 * returns node
 */
_BFTreeNode * _BFTreeNodeInsert(
	_BFTreeNode * node,
	BFTreeObject object,
	int (*compare)(BFTreeObject a, BFTreeObject b),
	unsigned char flags,
	int * error
);

/**
 * returns node
 */
_BFTreeNode * _BFTreeNodeRemove(
	_BFTreeNode * node,
	BFTreeObject object,
	int (*compare)(BFTreeObject a, BFTreeObject b),
	void (*release)(BFTreeObject object)
);

/**
 * returns node
 */
bool _BFTreeNodeSearch(
	_BFTreeNode * node,
	BFTreeObject obj,
	int (*compare)(BFTreeObject a, BFTreeObject b)
);

#endif // INTERNAL_TREE_H

