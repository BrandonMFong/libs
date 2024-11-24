/**
 * author: Brando
 * date: 11/22/24
 */

#ifndef TREE_TESTS_H
#define TREE_TESTS_H

#include "clib_tests.h"
#include "tree.h"

int BFTestTreeCompare(BFTreeNodeObject aobj, BFTreeNodeObject bobj) {
	int a = *(int *) aobj;
	int b = *(int *) bobj;
	return a - b;
}

void BFTestNodeRelease(BFTreeNodeObject obj) {
	BFFree(obj);
}

BFTEST_UNIT_FUNC(test_treeinit, 2<<10, {
	BFTree * tree = BFTreeCreate();
	tree->compare = BFTestTreeCompare;
	BF_ASSERT(tree, "a null tree was returned");
	BFTreeRelease(tree);
})

BFTEST_UNIT_FUNC(test_treenodeinit, 2<<10, {
	BFTreeNode * node = BFTreeNodeCreate();
	//node->release = BFTestNodeRelease;
	BF_ASSERT(node, "a null node was returned");
	BFTreeNodeRelease(node);
})

BFTEST_UNIT_FUNC(test_CreateNodeWithObject, 2<<10, {
	BFTreeNode * node = BFTreeNodeCreate();
	BF_ASSERT(node, "a null node was returned");
	//node->release = BFTestNodeRelease;

	BFRandInit(time(0));
	int * value = (int *) malloc(sizeof(int));
	*value = BFRand();
	node->object = value;

	BFTreeNodeRelease(node);
	BFFree(value);
})

void BFTestTreePrint(BFTreeNode * node) {
	if (node) {
		BFTestPrint(
			"node = object=%d height=%d ",
			* (int *) node->object,
			node->height
		);
		BFTestTreePrint(node->left);
		BFTestTreePrint(node->right);
	}
}

BFTEST_UNIT_FUNC(test_InsertNodes, 2<<10, {
	if (BFTEST_UNIT_FUNC_ITR == 0) {
		BFRandInit(time(0));
	}

	// create trees
	BFTree * tree = BFTreeCreate();
	BF_ASSERT(tree, "a null tree was returned");
	tree->compare = BFTestTreeCompare;

	// create nodes
	int treesize = 2<<9;
	int * objects[treesize];
	for (int i = 0; i < treesize; i++) {
		// create object
		objects[i] = (int *) malloc(sizeof(int));
		*objects[i] = abs(BFRand());

		// insert into tree
		int err = BFTreeInsert(tree, objects[i]);
		BF_ASSERT(err == 0, "node insertion failed, node(obj=%d)", i);
	}

	if (BFTEST_UNIT_FUNC_ITR == 0) {
		//BFTestTreePrint(tree->root);
	}

	BFTreeRelease(tree);

	for (int i = 0; i < treesize; i++) {
		BFFree(objects[i]);
	}
})

BFTEST_UNIT_FUNC(test_InsertNodesAndSearch, 2<<10, {
	if (BFTEST_UNIT_FUNC_ITR == 0) {
		BFRandInit(time(0));
	}

	// create trees
	BFTree * tree = BFTreeCreate();
	BF_ASSERT(tree, "a null tree was returned");
	tree->compare = BFTestTreeCompare;

	// create nodes
	int treesize = 2<<9;
	int * objects[treesize];
	for (int i = 0; i < treesize; i++) {
		// create object
		objects[i] = (int *) malloc(sizeof(int));
		*objects[i] = abs(BFRand());

		// insert into tree
		int err = BFTreeInsert(tree, objects[i]);
		BF_ASSERT(err == 0, "node insertion failed, node(obj=%d)", i);
	}

	if (BFTEST_UNIT_FUNC_ITR == 0) {
		//BFTestTreePrint(tree->root);
	}

	int randNumSearch = 20;
	while (randNumSearch--) {
		int index = abs(BFRand()) % treesize;
		int * object = objects[index];

		BFTreeNode * node = BFTreeGetNode(tree, object);
		BF_ASSERT(node != NULL, "a null node was returned");
	}

	BFTreeRelease(tree);

	for (int i = 0; i < treesize; i++) {
		BFFree(objects[i]);
	}
})

BFTEST_UNIT_FUNC(test_InsertAndRemovingNodes, 2<<10, {
	// create trees
	BFTree * tree = BFTreeCreate();
	BF_ASSERT(tree, "a null tree was returned");
	tree->compare = BFTestTreeCompare;

	// create nodes and insert
	int treesize = 2<<5;
	int * objects[treesize];
	for (int i = 0; i < treesize; i++) {
		// create object
		objects[i] = (int *) malloc(sizeof(int));
		*objects[i] = abs(BFRand());

		// insert into tree
		int err = BFTreeInsert(tree, objects[i]);
		BF_ASSERT(err == 0, "node insertion failed, node(obj=%d)", i);
	}

	BF_ASSERT(BFTreeSize(tree) == treesize, "tree.size=%d != %d", BFTreeSize(tree), treesize);

	// remove nodes
	int randNumSearch = 20;
	while (randNumSearch--) {
		int index = abs(BFRand()) % treesize;
		int * object = objects[index];

		BFTreeNode * node = BFTreeGetNode(tree, object);
		if (node) {
			int err = BFTreeRemoveNode(tree, node);
			BF_ASSERT(err == 0, "couldn't remove node for object=%d", *object);
			BFTreeNodeRelease(node);
		}
	}

	if (BFTEST_UNIT_FUNC_ITR == 0) {
		//BFTestTreePrint(tree->root);
	}

	BFTreeRelease(tree);
	
	for (int i = 0; i < treesize; i++) {
		BFFree(objects[i]);
	}
})

BFTEST_COVERAGE_FUNC(tree_tests, {
	BFTEST_LAUNCH(test_treeinit);
	BFTEST_LAUNCH(test_treenodeinit);
	BFTEST_LAUNCH(test_CreateNodeWithObject);
	BFTEST_LAUNCH(test_InsertNodes);
	BFTEST_LAUNCH(test_InsertNodesAndSearch);
	BFTEST_LAUNCH(test_InsertAndRemovingNodes);

})

#endif // TREE_TESTS_H

