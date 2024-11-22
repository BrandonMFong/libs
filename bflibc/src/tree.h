/**
 * author: brando
 * date: 11/22/24
 */

#ifndef TREE_H
#define TREE_H

typedef void * BFTreeNodeObject;

typedef struct BFTreeNode {
	struct BFTreeNode * left;
	struct BFTreeNode * right;
	void (*release)(BFTreeNodeObject obj);
	BFTreeNodeObject object;
} BFTreeNode;

BFTreeNode * BFTreeNodeCreate();
void BFTreeNodeRelease(BFTreeNode * node);

typedef struct BFTree {
	BFTreeNode * root;
	size_t size;
	int (*compare)(BFTreeNodeObject a, BFTreeNodeObject b);
} BFTree;

BFTree * BFTreeCreate();
void BFTreeRelease(BFTree * tree);
int BFTreeInsertNode(BFTree * tree, BFTreeNode * node);
int BFTreeRemoveNode(BFTree * tree, BFTreeNode * node);
BFTreeNode * BFTreeGetNodeForObject(BFTree * tree, BFTreeNodeObject obj);

#endif // TREE_H
