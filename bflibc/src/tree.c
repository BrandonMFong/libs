/**
 * author: brando
 * date: 11/22/24
 */

#include "tree.h"
#include "free.h"

BFTreeNode * BFTreeNodeCreate() {
	BFTreeNode * res = (BFTreeNode *) malloc(sizeof(BFTreeNode));
	return res;
}

void BFTreeNodeRelease(BFTreeNode * node) {
	BFFree(node);
}

BFTree * BFTreeCreate() {
	BFTree * res = (BFTree *) malloc(sizeof(BFTree));
	return res;
}

void BFTreeRelease(BFTree * tree) {
	BFFree(tree);
}

int BFTreeInsertNode(BFTree * tree, BFTreeNode * node) {
	return 0;
}

int BFTreeRemoveNode(BFTree * tree, BFTreeNode * node) {
	return 0;
}

BFTreeNode * BFTreeGetNodeForObject(BFTree * tree, BFTreeNodeObject obj) {
	return NULL;
}

