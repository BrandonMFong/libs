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
 * frees tree
 */
void BFTreeRelease(BFTree tree);

/**
 * number of nodes in tree
 */
size_t BFTreeSize(BFTree tree);

/**
 * node: will be ownd by tree
 */
int BFTreeInsert(BFTree tree, BFTreeNodeObject object);

/**
 * node will not be deleted, caller is responsible for calling it
 */
int BFTreeRemove(BFTree tree, BFTreeNodeObject object);

/**
 * returns NULL if node with `obj` could not be found
 */
bool BFTreeContains(BFTree tree, BFTreeNodeObject obj);

#endif // TREE_H

