/**
 * author: Brando
 * date: 11/22/24
 */

#ifndef TREE_TESTS_H
#define TREE_TESTS_H

#include "clib_tests.h"
#include "tree.h"
#include "internal/tree.h"
#include <stdlib.h>

int BFTestTreeCompare(BFTreeObject aobj, BFTreeObject bobj) {
	int a = *(int *) aobj;
	int b = *(int *) bobj;
	return a - b;
}

void BFTestNodeRelease(BFTreeObject obj) {
	BFFree(obj);
}

BFTEST_UNIT_FUNC(test_treeinit, 2<<10, {
	BFTree tree = BFTreeCreate();
	BFTreeSetCompare(tree, BFTestTreeCompare); 
	BF_ASSERT(tree, "a null tree was returned");
	BFTreeRelease(tree);
})

BFTEST_UNIT_FUNC(test_treenodeinit, 2<<10, {
	_BFTreeNode * node = _BFTreeNodeCreate();
	BF_ASSERT(node, "a null node was returned");
	_BFTreeNodeRelease(node);
})

BFTEST_UNIT_FUNC(test_CreateNodeWithObject, 2<<10, {
	_BFTreeNode * node = _BFTreeNodeCreate();
	BF_ASSERT(node, "a null node was returned");

	BFRandInit(time(0));
	int * value = (int *) malloc(sizeof(int));
	*value = BFRand();
	node->object = value;

	_BFTreeNodeRelease(node);
	BFFree(value);
})

void BFTestTreePrint(_BFTreeNode * node) {
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
	BFTree tree = BFTreeCreate();
	BF_ASSERT(tree, "a null tree was returned");
	BFTreeSetCompare(tree, BFTestTreeCompare); 
	BFTreeSetRelease(tree, free); 

	// create nodes
	int treesize = 2<<9;
	int * objects[treesize];
	for (int i = 0; i < treesize; i++) {
		// create object
		objects[i] = (int *) malloc(sizeof(int));
		*objects[i] = i;

		// insert into tree
		int err = BFTreeInsert(tree, objects[i]);
		BF_ASSERT(err == 0, "node insertion failed, node(obj=%d)", i);
	}

	if (BFTEST_UNIT_FUNC_ITR == 0) {
		//BFTestTreePrint(tree->root);
	}

	BFTreeRelease(tree);
})

BFTEST_UNIT_FUNC(test_InsertNodesAndSearch, 2<<10, {
	if (BFTEST_UNIT_FUNC_ITR == 0) {
		BFRandInit(time(0));
	}

	// create trees
	BFTree tree = BFTreeCreate();
	BF_ASSERT(tree, "a null tree was returned");
	BFTreeSetCompare(tree, BFTestTreeCompare); 
	BFTreeSetRelease(tree, free); 

	// create nodes
	int treesize = 2<<9;
	int * objects[treesize];
	for (int i = 0; i < treesize; i++) {
		// create object
		objects[i] = (int *) malloc(sizeof(int));
		*objects[i] = i;

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
		BF_ASSERT(BFTreeContains(tree, objects[index]), "a null node was returned");
	}

	BFTreeRelease(tree);
})

BFTEST_UNIT_FUNC(test_InsertAndRemovingNodes, 2<<10, {
	// create trees
	BFTree tree = BFTreeCreate();
	BF_ASSERT(tree, "a null tree was returned");
	BFTreeSetCompare(tree, BFTestTreeCompare); 
	BFTreeSetRelease(tree, free); 

	// create nodes and insert
	int treesize = 2<<5;
	int * objects[treesize];
	for (int i = 0; i < treesize; i++) {
		// create object
		objects[i] = (int *) malloc(sizeof(int));
		*objects[i] = i;

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

		if (BFTreeContains(tree, object)) {
			int err = BFTreeRemove(tree, object);
			BF_ASSERT(err == 0, "couldn't remove node for object=%d", *object);

			// clear from our object list since it has been released
			objects[index] = NULL;
		}
	}

	if (BFTEST_UNIT_FUNC_ITR == 0) {
		//BFTestTreePrint(tree->root);
	}

	BFTreeRelease(tree);
})

BFTEST_UNIT_FUNC(test_InsertDuplicates, 2<<10, {
	if (BFTEST_UNIT_FUNC_ITR == 0) {
		BFRandInit(time(0));
	}

	// create trees
	BFTree tree = BFTreeCreate();
	BF_ASSERT(tree, "a null tree was returned");
	BFTreeSetCompare(tree, BFTestTreeCompare); 
	BFTreeSetRelease(tree, free);

	int val = BFRand();

	// object 1
	int * object = (int *) malloc(sizeof(int));
	*object = val;
	int err = BFTreeInsert(tree, object);
	BF_ASSERT(err == 0, "error inserting %d", err);

	// object 2 using the same value
	object = (int *) malloc(sizeof(int));
	*object = val;
	err = BFTreeInsert(tree, object);
	BF_ASSERT(err != 0, "should be an error inserting %d since it is a duplicate", err);

	// now free since the tree doesn't hold onto it
	BFFree(object);

	if (BFTEST_UNIT_FUNC_ITR == 0) {
		//BFTestTreePrint(tree->root);
	}

	BFTreeRelease(tree);
})

int BFTestTreeCompareIntegers(BFTreeObject aobj, BFTreeObject bobj) {
	int a = (intptr_t) aobj;
	int b = (intptr_t) bobj;
	return a - b;
}

BFTEST_UNIT_FUNC(test_InsertAndRemovingNoPointers, 2<<10, {
	// create trees
	BFTree tree = BFTreeCreate();
	BF_ASSERT(tree, "a null tree was returned");
	BFTreeSetCompare(tree, BFTestTreeCompareIntegers); 

	// create nodes and insert
	int treesize = 2<<6;
	int objects[treesize];
	for (int i = 0; i < treesize; i++) {
		// create object
		objects[i] = i+1;

		// insert into tree
		int err = BFTreeInsert(tree, (BFTreeObject) (intptr_t) objects[i]);
		BF_ASSERT(err == 0, "node insertion failed, node(obj=%d)", i);
	}

	BF_ASSERT(BFTreeSize(tree) == treesize, "tree.size=%d != %d", BFTreeSize(tree), treesize);

	// remove nodes
	int randNumSearch = 20;
	while (randNumSearch--) {
		int index = abs(BFRand()) % treesize;
		int object = objects[index];

		if (BFTreeContains(tree, (BFTreeObject) (intptr_t) object)) {
			int err = BFTreeRemove(tree, (BFTreeObject) (intptr_t) object);
			BF_ASSERT(err == 0, "couldn't remove node for object=%d", object);

			// clear from our object list since it has been released
			objects[index] = 0;
		}
	}

	if (BFTEST_UNIT_FUNC_ITR == 0) {
		//BFTestTreePrint(tree->root);
	}

	BFTreeRelease(tree);
})

BFTEST_UNIT_FUNC(test_treeGettingNonexistentValues, 2<<10, {
	// create trees
	BFTree tree = BFTreeCreate();
	BF_ASSERT(tree, "a null tree was returned");
	BFTreeSetCompare(tree, BFTestTreeCompareIntegers); 

	// create nodes and insert
	int treesize = 2<<6;
	int objects[treesize];
	for (int i = 0; i < treesize; i += 2) {
		// create object
		objects[i] = i;

		// insert into tree
		int err = BFTreeInsert(tree, (BFTreeObject) (intptr_t) objects[i]);
		BF_ASSERT(err == 0, "node insertion failed, node(obj=%d)", i);
	}

	BF_ASSERT(BFTreeSize(tree) == (treesize / 2), "tree.size=%d != %d", BFTreeSize(tree), treesize);

	for (int i = 1; i < treesize; i += 2) {
		BF_ASSERT(
			!BFTreeContains(tree, (BFTreeObject) (intptr_t) i), 
			"there should not be an object of value %d", objects[i]);
	}

	if (BFTEST_UNIT_FUNC_ITR == 0) {
		//BFTestTreePrint(tree->root);
	}

	BFTreeRelease(tree);
})


BFTEST_COVERAGE_FUNC(tree_tests, {
	BFTEST_LAUNCH(test_treeinit);
	BFTEST_LAUNCH(test_treenodeinit);
	BFTEST_LAUNCH(test_CreateNodeWithObject);
	BFTEST_LAUNCH(test_InsertNodes);
	BFTEST_LAUNCH(test_InsertNodesAndSearch);
	BFTEST_LAUNCH(test_InsertAndRemovingNodes);
	BFTEST_LAUNCH(test_InsertDuplicates);
	BFTEST_LAUNCH(test_InsertAndRemovingNoPointers);
	BFTEST_LAUNCH(test_treeGettingNonexistentValues);

})

#endif // TREE_TESTS_H

