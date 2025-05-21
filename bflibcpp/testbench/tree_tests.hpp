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

int BFTestTreeCompare(const int & a, const int & b) {
	return a - b;
}

void BFTestTreeRelease(int object) { }

BFTEST_UNIT_FUNC(test_treeInit, 2<<10,  {
	Tree<int> tree;
	tree.setRelease(BFTestTreeRelease);
})

BFTEST_UNIT_FUNC(test_treeInsert, 2<<10,  {
	Tree<int> tree;
	tree.setRelease(BFTestTreeRelease);

	int treesize = 2<<8;
	for (int i = 0; i < treesize; i++) {
		int err = tree.insert(i);
		BF_ASSERT(err == 0, "itr=%d, couldn't insert %d, err=%d", BFTEST_UNIT_FUNC_ITR, i, err);
	}
	BF_ASSERT(tree.size() == treesize, "%ld != %ld", tree.size(), treesize);
})

BFTEST_UNIT_FUNC(test_treeRemove, 2<<10, {
	if (BFTEST_UNIT_FUNC_ITR == 0) {
		BFRandInit(time(0));
	}
	Tree<int> tree;
	tree.setRelease(BFTestTreeRelease);

	int treesize = 2<<8;
	for (int i = 0; i < treesize; i++) {
		BF_ASSERT(!tree.insert(i), "couldn't insert %d", i);
	}
	BF_ASSERT(tree.size() == treesize, "%ld != %ld", tree.size(), treesize);

	// remove
	for (int i = 0; i < 20; i++) {
		int value = BFMathAbs(BFRand()) % treesize;
		if (tree.contains(value)) {
			BF_ASSERT(!tree.remove(value));
		}
	}
})

BFTEST_UNIT_FUNC(test_treeContains, 2<<10,  {
	Tree<int> tree;
	tree.setRelease(BFTestTreeRelease);

	int treesize = 2<<8;
	for (int i = 0; i < treesize; i += 2) {
		BF_ASSERT(!tree.insert(i), "couldn't insert %d", i);
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

template<typename T>
void _BFTestTreeTraverseInorder(const typename Tree<T>::Node node) {
	if (node.isNull()) return;
	_BFTestTreeTraverseInorder<T>(node.left());
	if (node.object() != 0) {
		// TODO: assert nonzero value
	}
	_BFTestTreeTraverseInorder<T>(node.right());
}

template<typename T>
void _BFTestTreeTraversePreorder(const typename Tree<T>::Node node) {
	if (node.isNull()) return;
	if (node.object() != 0) {
		// TODO: assert nonzero value
	}
	_BFTestTreeTraverseInorder<T>(node.left());
	_BFTestTreeTraverseInorder<T>(node.right());
}

template<typename T>
void _BFTestTreeTraversePostorder(const typename Tree<T>::Node node) {
	if (node.isNull()) return;
	_BFTestTreeTraverseInorder<T>(node.left());
	_BFTestTreeTraverseInorder<T>(node.right());
	if (node.object() != 0) {
		// TODO: assert nonzero value
	}
}

BFTEST_UNIT_FUNC(test_treeTraversing, 2<<10,  {
	Tree<int> tree;
	tree.setRelease(BFTestTreeRelease);

	int treesize = 2<<8;
	for (int i = 0; i < treesize; i += 2) {
		BF_ASSERT(!tree.insert(i), "couldn't insert %d", i);
	}
	BF_ASSERT(tree.size() == treesize/2, "%ld != %ld", tree.size(), treesize);

	const Tree<int>::Node root = tree.root();
	_BFTestTreeTraverseInorder<int>(root);
	_BFTestTreeTraversePreorder<int>(root);
	_BFTestTreeTraversePostorder<int>(root);
})

int BFTestTreeComparePointers(int * const &ap, int * const &bp) {
	if (!ap || !bp) return -1;
	int a = *ap;
	int b = *bp;
	return a - b;
}

void BFTestTreeReleasePointer(int * object) {
	BFFree(object);
}

BFTEST_UNIT_FUNC(test_treeWithMallocObjects, 2<<10, {
	if (BFTEST_UNIT_FUNC_ITR == 0) {
		BFRandInit(time(0));
	}
	Tree<int*> tree;
	tree.setRelease(BFTestTreeReleasePointer);

	int treesize = 2<<8;
	for (int i = 0; i < treesize; i++) {
		int * object = (int *) malloc(sizeof(int));
		*object = i;
		BF_ASSERT(!tree.insert(object));
	}
	BF_ASSERT(tree.size() == treesize, "%ld != %ld", tree.size(), treesize);

	// remove
	for (int i = 0; i < 20; i++) {
		int value = BFMathAbs(BFRand()) % treesize;
		if (tree.contains(&value)) {
			BF_ASSERT(!tree.remove(&value));
		}
	}
})

String _BFTreeTestsCreateRandomWord() {
	const char charset[] = "abcdefghijklmnopqrstuvwxyz";
	size_t charset_size = strlen(charset);

	srand(time(0));

	size_t reslen = 2 << 3;
	char word[reslen + 1];
	for (int i = 0; i < reslen; i++) {
		word[i] = charset[rand() % charset_size];
	}

	word[reslen] = '\0';

	return word;
}

BFTEST_UNIT_FUNC(test_treeWithStrings, 1, {
	Tree<String> tree(true);

	BF_ASSERT(tree.allowDuplicates());

	int treesize = 2;
	for (int i = 0; i < treesize; i++) {
		String word = _BFTreeTestsCreateRandomWord();
		BFTestPrint("inserting %s", word.cString());
		int err = tree.insert(word);
		BF_ASSERT(err == 0, "itr=%d, couldn't insert '%s', err=%d", BFTEST_UNIT_FUNC_ITR, word.cString(), err);
	}
	BF_ASSERT(tree.size() == treesize, "%ld != %ld", tree.size(), treesize);
})

void _BFTestTreeRelease(char * str) {
	free(str);
}

BFTEST_UNIT_FUNC(test_treeWithCustomCompare, 1, {
	Tree<char *> tree(true);
	tree.setRelease(_BFTestTreeRelease);

	BF_ASSERT(tree.allowDuplicates());

	int treesize = 2<<8;
	for (int i = 0; i < treesize; i++) {
		char * word = _BFTreeTestsCreateRandomWord().cStringCopy();
		int err = tree.insert(word);
		BF_ASSERT(err == 0, "itr=%d, couldn't insert '%s', err=%d", BFTEST_UNIT_FUNC_ITR, word, err);
	}
	BF_ASSERT(tree.size() == treesize, "%ld != %ld", tree.size(), treesize);
})

BFTEST_COVERAGE_FUNC(tree_tests, {
	/*
	BFTEST_LAUNCH(test_treeInit);
	BFTEST_LAUNCH(test_treeInsert);
	BFTEST_LAUNCH(test_treeRemove);
	BFTEST_LAUNCH(test_treeContains);
	BFTEST_LAUNCH(test_treeTraversing);
	BFTEST_LAUNCH(test_treeWithMallocObjects);
	*/
	BFTEST_LAUNCH(test_treeWithStrings);
	//BFTEST_LAUNCH(test_treeWithCustomCompare);

})

#endif // TREE_TESTS_HPP

