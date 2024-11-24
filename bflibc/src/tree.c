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
	//res->release = NULL;
	res->left = NULL;
	res->right = NULL;
	res->object = NULL;
	res->height = 1;
	return res;
}

void BFTreeNodeRelease(BFTreeNode * node) {
	/*
	if (node && node->release) {
		node->release(node->object);
		node->object = NULL;
	}
	*/
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

#include <bftest/bftest.h>
void BFTreeRelease(BFTree * tree) {
	BFTreeReleaseNode(tree, tree->root);
	if (tree->size > 0) {
		BFTestPrint("size=%d", tree->size);
	}
	BFFree(tree);
}

size_t BFTreeSize(BFTree * tree) {
	if (!tree) return 0;
	return tree->size;
}

int BFTreeInsertNode(BFTree * tree, BFTreeNode * node) {
	if (!tree || !node) {
		return -1;
	}
	tree->root = BFTreeInsert(tree->root, node, tree->compare);
	tree->size++;
	return 0;
}

int BFTreeRemoveNode(BFTree * tree, BFTreeNode * node) {
	if (!tree || !node) {
		return -1;
	}
	tree->root = BFTreeNodeRemove(tree->root, node, tree->compare);
	tree->size--;
	return 0;
}

BFTreeNode * BFTreeGetNode(BFTree * tree, BFTreeNodeObject obj) {
	if (!tree) {
		return NULL;
	} else if (obj == NULL) {
		return NULL;
	}
	return BFTreeNodeSearch(tree->root, obj, tree->compare);
}

