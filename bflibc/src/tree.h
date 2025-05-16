/**
 * author: brando
 * date: 11/22/24
 *
 * binary search tree (avl implementation)
 */

#ifndef TREE_H
#define TREE_H

#include <stddef.h>
#include <stdbool.h>

typedef void * BFTreeObject;
typedef void * BFTreeNode;
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
void BFTreeSetCompare(BFTree tree, int (*compare)(BFTreeObject a, BFTreeObject b));

/**
 * defines how each object is deleted when the node is 
 * deleted
 */
void BFTreeSetRelease(BFTree tree, void (*release)(BFTreeObject object));

/**
 * allow == true to permit the use of duplicates in this binary tree
 *
 * by default, this binary tree does NOT allow duplicates.
 */
void BFTreeSetAllowDuplicates(BFTree tree, bool allow);

/**
 * returns true if duplicates are allowed
 */
bool BFTreeGetAllowDuplicates(BFTree tree);

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
 * object: pointer or integer value. value=0 is allowed
 *
 * returns: -1 if object couldn't be inserted. Object may be a duplicate
 */
int BFTreeInsert(BFTree tree, BFTreeObject object);

/**
 * will release the object memory if BFTreeSetRelease was defined
 */
int BFTreeRemove(BFTree tree, BFTreeObject object);

/**
 * returns false if node with `obj` could not be found
 */
bool BFTreeContains(BFTree tree, BFTreeObject obj);

/**
 * returns tree's root
 *
 * will be empty if BFTreeSize() == 0
 */
BFTreeNode BFTreeGetRoot(BFTree tree);

/**
 * returns node's left child
 */
BFTreeNode BFTreeNodeGetLeft(BFTreeNode node);

/**
 * returns node's right child
 */
BFTreeNode BFTreeNodeGetRight(BFTreeNode node);

/**
 * returns node's object
 */
BFTreeObject BFTreeNodeGetObject(BFTreeNode node);

#endif // TREE_H

