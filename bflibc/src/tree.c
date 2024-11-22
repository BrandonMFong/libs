/**
 * author: brando
 * date: 11/22/24
 */

#include "tree.h"
#include "free.h"
#include "internal/tree.h"

BFTreeNode * BFTreeNodeCreate() {
	BFTreeNode * res = (BFTreeNode *) malloc(sizeof(BFTreeNode));
	res->release = NULL;
	res->left = NULL;
	res->right = NULL;
	res->object = NULL;
	res->height = 0;
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

// left->right->node
void BFTreeReleaseNode(BFTreeNode * node) {
	if (!node) return;
	BFTreeReleaseNode(node->left);
	BFTreeReleaseNode(node->right);
	BFTreeNodeRelease(node);
}

void BFTreeRelease(BFTree * tree) {
	BFTreeReleaseNode(tree->root);
	BFFree(tree);
}

int BFTreeInsertNode(BFTree * tree, BFTreeNode * node) {
	if (!tree || !node) {
		return -1;
	}

	tree->root = BFTreeInsert(tree->root, node, tree->compare);

	return 0;
}

int BFTreeRemoveNode(BFTree * tree, BFTreeNode * node) {
	return 0;
}

BFTreeNode * BFTreeGetNodeForObject(BFTree * tree, BFTreeNodeObject obj) {
	return NULL;
}

