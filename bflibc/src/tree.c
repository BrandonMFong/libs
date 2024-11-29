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

void BFTreeSetCompare(BFTree tree, int (*compare)(BFTreeObject a, BFTreeObject b)) {
	if (!tree) return;
	((_BFTree *) tree)->compare = compare;
}

void BFTreeSetRelease(BFTree tree, void (*release)(BFTreeObject object)) {
	if (!tree) return;
	((_BFTree *) tree)->release = release;
}

// left->right->node
void BFTreeReleaseNode(_BFTree * tree, _BFTreeNode * node) {
	if (!node) return;
	BFTreeReleaseNode(tree, node->left);
	BFTreeReleaseNode(tree, node->right);

	/*
	if (tree->release) {
		tree->release(node->object);
	}
	*/
	_BFTreeNodeRelease(node);
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

int BFTreeInsert(BFTree _tree, BFTreeObject object) {
	if (!_tree) {
		return -1;
	}
	_BFTree * tree = (_BFTree *) _tree;
	int err = 0;
	tree->root = _BFTreeNodeInsert(
		tree->root,
		object,
		tree->compare,
		&err
	);

	if (err == 0) {
		tree->size++;
	}

	return err;
}

int BFTreeRemove(BFTree _tree, BFTreeObject object) {
	if (!_tree || !object) {
		return -1;
	}
	_BFTree * tree = (_BFTree *) _tree;
	tree->root = _BFTreeNodeRemove(
		tree->root, object, tree->compare, tree->release
	);

	if (tree->release) {
		tree->release(object);
	}

	tree->size--;
	return 0;
}

bool BFTreeContains(BFTree _tree, BFTreeObject object) {
	_BFTree * tree = (_BFTree *) _tree;
	if (!tree) {
		return false;
	}
	return _BFTreeNodeSearch(tree->root, object, tree->compare);
}

BFTreeNode BFTreeGetRoot(BFTree _tree) {
	_BFTree * tree = (_BFTree *) _tree;
	if (!tree) {
		return NULL;
	}
	return tree->root;
}

BFTreeNode BFTreeNodeGetLeft(BFTreeNode _node) {
	_BFTreeNode * node = (_BFTreeNode *) _node;
	if (!node) {
		return NULL;
	}
	return node->left;
}

BFTreeNode BFTreeNodeGetRight(BFTreeNode _node) {
	_BFTreeNode * node = (_BFTreeNode *) _node;
	if (!node) {
		return NULL;
	}
	return node->right;
}

BFTreeObject BFTreeNodeGetObject(BFTreeNode _node) {
	_BFTreeNode * node = (_BFTreeNode *) _node;
	if (!node) {
		return NULL;
	}
	return node->object;
}
