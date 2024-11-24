/**
 * author: brando
 * date: 11/22/24
 */

#include "tree.h"
#include "free.h"
#include "internal/tree.h"
#include <stdio.h>

BFTreeNode * BFTreeNodeCreate() {
	BFTreeNode * res = (BFTreeNode *) malloc(sizeof(BFTreeNode));
	res->left = NULL;
	res->right = NULL;
	res->object = NULL;
	res->height = 1;
	return res;
}

void BFTreeNodeRelease(BFTreeNode * node) {
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
void BFTreeReleaseNode(BFTree * tree, BFTreeNode * node) {
	if (!node) return;
	BFTreeReleaseNode(tree, node->left);
	BFTreeReleaseNode(tree, node->right);
	BFTreeNodeRelease(node);
	tree->size--;
}

void BFTreeRelease(BFTree * tree) {
	BFTreeReleaseNode(tree, tree->root);
	BFFree(tree);
}

size_t BFTreeSize(BFTree * tree) {
	if (!tree) return 0;
	return tree->size;
}

int BFTreeInsert(BFTree * tree, BFTreeNodeObject object) {
	if (!tree || !object) {
		return -1;
	}
	tree->root = BFTreeNodeInsert(tree->root, object, tree->compare);
	tree->size++;
	return 0;
}

int BFTreeRemove(BFTree * tree, BFTreeNodeObject object) {
	if (!tree || !object) {
		return -1;
	}
	tree->root = BFTreeNodeRemove(tree->root, object, tree->compare);
	tree->size--;
	return 0;
}

bool BFTreeContains(BFTree * tree, BFTreeNodeObject object) {
	if (!tree || !object) {
		return -1;
	}
	return BFTreeNodeSearch(tree->root, object, tree->compare);
}

