/**
 * author: Brando
 * date: 11/28/24
 */

#ifndef TREE_TESTS_HPP
#define TREE_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include "tree.hpp"
#include "string.hpp"

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

int BFTestTreeCompare(int & a, int & b) {
	return a - b;
}

void BFTestTreeRelease(int object) { }

BFTEST_UNIT_FUNC(test_treeInit, 2<<10,  {
	Tree<int> tree;
	tree.setCompare(BFTestTreeCompare);
	tree.setRelease(BFTestTreeRelease);
})

BFTEST_UNIT_FUNC(test_treeInsert, 2<<10,  {
	Tree<int> tree;
	tree.setCompare(BFTestTreeCompare);
	tree.setRelease(BFTestTreeRelease);

	int treesize = 2<<8;
	for (int i = 0; i < treesize; i++) {
		BF_ASSERT(!tree.insert(i));
	}
	BF_ASSERT(tree.size() == treesize, "%ld != %ld", tree.size(), treesize);
})

BFTEST_UNIT_FUNC(test_treeRemove, 2<<10,  {
	Tree<int> tree;
	tree.setCompare(BFTestTreeCompare);
	tree.setRelease(BFTestTreeRelease);

	int treesize = 2<<8;
	for (int i = 0; i < treesize; i++) {
		BF_ASSERT(!tree.insert(i));
	}
	BF_ASSERT(tree.size() == treesize, "%ld != %ld", tree.size(), treesize);

	// remove
	for (int i = 0; i < treesize; i++) {
		BF_ASSERT(!tree.remove(i));
	}
	BF_ASSERT(tree.size() == 0, "size is %ld", tree.size());
})

BFTEST_UNIT_FUNC(test_treeContains, 2<<10,  {
	Tree<int> tree;
	tree.setCompare(BFTestTreeCompare);
	tree.setRelease(BFTestTreeRelease);

	int treesize = 2<<8;
	for (int i = 0; i < treesize; i += 2) {
		BF_ASSERT(!tree.insert(i));
	}
	BF_ASSERT(tree.size() == treesize/2, "%ld != %ld", tree.size(), treesize);

	// contains
	for (int i = 0; i < treesize; i++) {
		if (i % 2 == 0) {
			BF_ASSERT(tree.contains(i));
		} else {
			BF_ASSERT(!tree.contains(i));
		}
	}
})

BFTEST_COVERAGE_FUNC(tree_tests, {
	BFTEST_LAUNCH(test_treeInit);
	BFTEST_LAUNCH(test_treeInsert);
	BFTEST_LAUNCH(test_treeRemove);
	BFTEST_LAUNCH(test_treeContains);

})

#endif // TREE_TESTS_HPP

