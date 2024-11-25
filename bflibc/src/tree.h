/**
 * author: brando
 * date: 11/22/24
 */

#ifndef TREE_H
#define TREE_H

#include <stddef.h>
#include <stdbool.h>

typedef void * BFTreeNodeObject;
typedef void * BFTree;

/**
 * mallocs BFTree
 */
BFTree BFTreeCreate();

/**
 * compare: how each object is compared to each other
 *	a < b -> result < 0 
 *	a > b -> result > 0
 *	a == b -> result == 0
 */
void BFTreeSetCompare(BFTree tree, int (*compare)(BFTreeNodeObject a, BFTreeNodeObject b));

/**
 * defines how each object is deleted when the node is 
 * deleted
 */
void BFTreeSetRelease(BFTree tree, void (*release)(BFTreeNodeObject object));

/**
 * frees tree
 *
 * uses the release callback from BFTreeSetRelease to release all objects inserted
 * inserted into the tree. If nothing was set, then we are assuming
 * the caller will handle the memory
 */
void BFTreeRelease(BFTree tree);

/**
 * number of nodes in tree
 */
size_t BFTreeSize(BFTree tree);

/**
 * no duplicates allowed 
 *
 * node: will be owned by tree
 *
 * returns: -1 if object couldn't be inserted. Object may be a duplicate
 */
int BFTreeInsert(BFTree tree, BFTreeNodeObject object);

/**
 * will release the object memory if BFTreeSetRelease was defined
 */
int BFTreeRemove(BFTree tree, BFTreeNodeObject object);

/**
 * returns false if node with `obj` could not be found
 */
bool BFTreeContains(BFTree tree, BFTreeNodeObject obj);

#endif // TREE_H

