/**
 * author: brando
 * date: 11/22/24
 */

#include "tree.h"
#include "free.h"

BFTreeNode * BFTreeNodeCreate() {
	BFTreeNode * res = (BFTreeNode *) malloc(sizeof(BFTreeNode));
	res->release = NULL;
	res->left = NULL;
	res->right = NULL;
	res->object = NULL;
	return res;
}

void BFTreeNodeRelease(BFTreeNode * node) {
	if (node && node->release) {
		node->release(node->object);
	}
	BFFree(node);
}

BFTree * BFTreeCreate() {
	BFTree * res = (BFTree *) malloc(sizeof(BFTree));
	res->root = NULL;
	res->compare = NULL;
	res->size = 0;
	return res;
}

void BFTreeRelease(BFTree * tree) {
	BFFree(tree);
}

BFTreeNode * _BFTreeInsertNode(BFTree * tree, BFTreeNode * node) {

}

int BFTreeInsertNode(BFTree * tree, BFTreeNode * node) {
	if (!tree || !node) {
		return -1;
	}

	if (!tree->root) { // empty tree
		tree->root = node;
	} else {
		
	}

	return 0;
}

int BFTreeRemoveNode(BFTree * tree, BFTreeNode * node) {
	return 0;
}

BFTreeNode * BFTreeGetNodeForObject(BFTree * tree, BFTreeNodeObject obj) {
	return NULL;
}

