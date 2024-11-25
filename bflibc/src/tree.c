/**
 * author: brando
 * date: 11/22/24
 */

#include "tree.h"
#include "free.h"
#include "internal/tree.h"
#include <stdio.h>

BFTree BFTreeCreate() {
	_BFTree * res = (_BFTree *) malloc(sizeof(_BFTree));
	res->root = NULL;
	res->compare = NULL;
	res->release = NULL;
	res->size = 0;
	return (BFTree) res;
}

void BFTreeSetCompare(BFTree tree, int (*compare)(BFTreeNodeObject a, BFTreeNodeObject b)) {
	if (!tree) return;
	((_BFTree *) tree)->compare = compare;
}

void BFTreeSetRelease(BFTree tree, void (*release)(BFTreeNodeObject object)) {
	if (!tree) return;
	((_BFTree *) tree)->release = release;
}

// left->right->node
void BFTreeReleaseNode(_BFTree * tree, BFTreeNode * node) {
	if (!node) return;
	BFTreeReleaseNode(tree, node->left);
	BFTreeReleaseNode(tree, node->right);

	if (tree->release) {
		tree->release(node->object);
	}
	BFTreeNodeRelease(node);
	tree->size--;
}

void BFTreeRelease(BFTree _tree) {
	_BFTree * tree = (_BFTree *) _tree;
	BFTreeReleaseNode(tree, tree->root);
	BFFree(tree);
}

size_t BFTreeSize(BFTree tree) {
	if (!tree) return 0;
	return ((_BFTree *) tree)->size;
}

int BFTreeInsert(BFTree _tree, BFTreeNodeObject object) {
	if (!_tree || !object) {
		return -1;
	}
	_BFTree * tree = (_BFTree *) _tree;
	int err = 0;
	tree->root = BFTreeNodeInsert(tree->root, object, tree->compare, &err);

	if (err == 0) {
		tree->size++;
	}

	return err;
}

int BFTreeRemove(BFTree _tree, BFTreeNodeObject object) {
	if (!_tree || !object) {
		return -1;
	}
	_BFTree * tree = (_BFTree *) _tree;
	tree->root = BFTreeNodeRemove(tree->root, object, tree->compare);

	if (tree->release) {
		tree->release(object);
	}

	tree->size--;
	return 0;
}

bool BFTreeContains(BFTree _tree, BFTreeNodeObject object) {
	if (!_tree || !object) {
		return false;
	}
	_BFTree * tree = (_BFTree *) _tree;
	return BFTreeNodeSearch(tree->root, object, tree->compare);
}

