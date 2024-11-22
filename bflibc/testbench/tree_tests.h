/**
 * author: Brando
 * date: 11/22/24
 */

#ifndef TREE_TESTS_H
#define TREE_TESTS_H

#include "clib_tests.h"
#include "tree.h"

int BFTestTreeCompare(BFTreeNodeObject a, BFTreeNodeObject b) {
	return 0;
}

void BFTestNodeRelease(BFTreeNodeObject obj) {
	BFTestPrint("freeing");
	free(obj);
}

BFTEST_UNIT_FUNC(test_treeinit, 2<<10, {
	BFTree * tree = BFTreeCreate();
	tree->compare = BFTestTreeCompare;
	BF_ASSERT(tree, "a null tree was returned");
	BFTreeRelease(tree);
})

BFTEST_UNIT_FUNC(test_treenodeinit, 2<<10, {
	BFTreeNode * node = BFTreeNodeCreate();
	node->release = BFTestNodeRelease;
	BF_ASSERT(node, "a null node was returned");
	BFTreeNodeRelease(node);
})

BFTEST_UNIT_FUNC(test_CreateNodeWithObject, 2<<10, {
	BFTreeNode * node = BFTreeNodeCreate();
	BF_ASSERT(node, "a null node was returned");
	node->release = BFTestNodeRelease;

	BFRandInit(time(0));
	int * value = (int *) malloc(sizeof(int));
	*value = BFRand();
	node->object = value;

	BFTreeNodeRelease(node);
})

BFTEST_UNIT_FUNC(test_InsertNodes, 2<<10, {
	// create trees
	BFTree * tree = BFTreeCreate();
	BF_ASSERT(tree, "a null tree was returned");
	tree->compare = BFTestTreeCompare;

	// create nodes
	int treesize = 10;
	for (int i = 0; i < treesize; i++) {
		// create node
		BFTreeNode * node = BFTreeNodeCreate();
		BF_ASSERT(node, "a null node was returned");
		node->release = BFTestNodeRelease;
		
		// set the value
		int * value = (int *) malloc(sizeof(int));
		*value = i;
		node->object = value;

		// insert into tree
		int err = BFTreeInsertNode(tree, node);
		BF_ASSERT(err == 0, "node insertion failed, node(obj=%d)", i);
	}

	BFTreeRelease(tree);
})

BFTEST_COVERAGE_FUNC(tree_tests, {
	BFTEST_LAUNCH(test_treeinit);
	BFTEST_LAUNCH(test_treenodeinit);
	BFTEST_LAUNCH(test_CreateNodeWithObject);
})

#endif // TREE_TESTS_H

