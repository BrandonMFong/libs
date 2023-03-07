/**
 * author: Brando
 * date: 9/7/22
 */

#ifndef BINTREE_TESTS_HPP
#define BINTREE_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include <bintree.hpp>
#include "cpplib_tests.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.hpp"

using namespace BF;

static int test_BinInitializer() {
	PRINT_TEST_RESULTS(1);

	BinTree<int> i;
	BinTree<double> d;
	BinTree<float> f;
	BinTree<char> c;
	
	BinTree<int> * ii = new BinTree<int>;
	BinTree<double> * dd = new BinTree<double>;
	BinTree<float> * ff = new BinTree<float>;
	BinTree<char> * cc = new BinTree<char>;

	Delete(ii);
	Delete(dd);
	Delete(ff);
	Delete(cc);

	return 0;
}

static int test_BinInsert() {
	int result = 0;

	BinTree<int> t;

	result = t.insert(2);
	
	if (result == 0) {
		result = t.insert(1);
	}
	
	if (result == 0) {
		result = t.insert(8);
	}
	
	if (result == 0) {
		result = t.insert(5);
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

static int test_BinMax() {
	int result = 0;

	BinTree<int> t;
	t.insert(1);
	t.insert(2);
	t.insert(3);
	t.insert(4);
	t.insert(5);
	t.insert(8);

	if (t.max() != 8) {
		result = 1;
		printf("Max is not: %d\n", t.max());
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

static int test_BinMin() {
	int result = 0;

	BinTree<int> t;
	t.insert(1);
	t.insert(2);
	t.insert(3);
	t.insert(4);
	t.insert(5);
	t.insert(8);

	if (t.min() != 1) {
		result = 1;
		printf("Min is not: %d\n", t.min());
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

static int test_BinNodeKnowsItsLeaf() {
	int result = 0;

	BinTree<int>::BinNode n1;
	BinTree<int>::BinNode n2;
	BinTree<int>::BinNode n3;

	if (!n1.isLeaf()) {
		result = 1;
		printf("Node should be leaf\n");
	}

	if (result == 0) {
		n1.setLeft(&n2);
		n1.setRight(&n3);

		if (n1.isLeaf()) {
			result = 2;
			printf("Node should not be leaf\n");
		}
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

static int test_BSTDelete() {
	int result = 0;

	BinTree<int> t;
	t.insert(5);
	t.insert(4);
	t.insert(6);
	t.insert(10);
	t.insert(12);
	t.insert(11);
	t.insert(13);
	t.insert(1);
	t.insert(3);

	result = t.remove(3);
	if (result) printf("Error removing 3\n");

	if (result == 0) {
		result = t.remove(10);
		if (result) printf("Error removing 10\n");
	}
	
	if (result == 0) {
		result = t.remove(5);
		if (result) printf("Error removing 8\n");
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_RemovingRoot() {
	int result = 0;

	BinTree<int> t0;
	BinTree<int> t1;
	BinTree<int> t2;
	t0.insert(2);
	t1.insert(2);
	t1.insert(3);
	t2.insert(2);
	t2.insert(1);
	t2.insert(3);

	result = t0.remove(2);

	if (result == 0) {
		result = t1.remove(2);
	}

	if (result == 0) {
		result = t2.remove(2);
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_RemovingAll() {
	int result = 0;

	BinTree<int> t;
	t.insert(5);
	t.insert(4);
	t.insert(6);
	t.insert(10);
	t.insert(8);
	t.insert(7);
	t.insert(9);
	t.insert(1);
	t.insert(3);

	result = t.removeAll();

	if (result == 0) {
		if (t.count()) {
			result = 1;
			printf("Count is %d\n", t.count());
		}
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_ReturningLeafValues() {
	int result = 0;

	BinTree<int> t;
	t.insert(5);
	t.insert(4);
	t.insert(6);

	List<int> l;
	
	result = t.leafValues(&l);

	if (result == 0) {
		if (l.count() != 2) {
			printf("List count is: %d\n", l.count());
			result = 1;
		} else if (!l.contains(4)) {
			printf("List does not have: 4\n");
			result = 2;
		} else if (!l.contains(6)) {
			printf("List does not have: 6\n");
			result = 6;
		}
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_BinTreeCount() {
	int result = 0;
	BinTree<int> t;
	const int size = 100;
	int counter = size;

	while (counter) {
		int val = rand() % 100;
		result = t.insert(val);
		if (result != 0) break;
		counter--;
	}

	if (result == 0) {
		if (t.count() != size) {
			printf("Size: %d, actual count: %d\n", size, t.count());
			result = 2;
		}
	}
	
	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_ReplacingBinNodes() {
	int result = 0;

	BinTree<char> t;

	BinTree<char>::BinNode * a = new BinTree<char>::BinNode('a');
	BinTree<char>::BinNode * b = new BinTree<char>::BinNode('b');
	BinTree<char>::BinNode * c = new BinTree<char>::BinNode('c');
	BinTree<char>::BinNode * d = new BinTree<char>::BinNode('d');
	BinTree<char>::BinNode * e = new BinTree<char>::BinNode('e');
	BinTree<char>::BinNode * f = new BinTree<char>::BinNode('f');
	BinTree<char>::BinNode * g = new BinTree<char>::BinNode('g');

	g->setLeft(f);

	a->setLeft(b);
	a->setRight(c);
	c->setLeft(d);
	c->setRight(e);

	t.replaceNodeWithNode(c, f);

	if (c->_parent) {
		printf("parent node should be null\n");
		result = 1;
	} else if (c->_location) {
		printf("location should be null\n");
		result = 2;
	} else if (f->_parent != a) {
		printf("Error swapping parents\n");
		result = 3;
	}

	Delete(a);
	Delete(b);
	Delete(c);
	Delete(d);
	Delete(e);
	Delete(f);
	Delete(g);
	
	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_SearchingBinTree() {
	int result = 0;

	int target = 4;
	BinTree<int> t;
	t.insert(5);
	t.insert(3);
	t.insert(8);
	t.insert(7);
	t.insert(target);
	t.insert(1);
	t.insert(2);
	t.insert(9);
	t.insert(0);
	t.insert(11);

	const BinTree<int>::BinNode * node = t.root();

	bool found = false;
	do {
		int obj = node->object();
		if (target < obj) node = node->left();
		else if (target > obj) node = node->right();
		else found = true;
	} while (!found && (node != NULL));

	if (!found) {
		result = 1;
		printf("Could not find %d\n", target);
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_Iterations() {
	int result = 0;

	// randomly add to list so we can have a better chance of a balanced tree
	List<int> l;

	srand(time(NULL));

	int size = 100, max = 100;
	for (int i = 0; i < size; i++) {
		int num = rand() % max;
		result = l.add(num);
		if (result) break;
	}

	BinTree<int> t;
	if (!result) {
		List<int>::Node * n = l.first();
		for(; n; n = n->next()) {
			result = t.insert(n->object());
			if (result) break;
		}
	}

	if (!result) {
		result = l.sort();
	}
	
	BinTree<int>::Iterator * itr = 0;
	if (!result) {
		result = t.createIterator(&itr);
	}

	List<int>::Node * n = l.first();
	while (!result && itr->finished() && n) {
		if (n->object() != itr->current()) {
			printf("%d != %d\n", n->object(), itr->current());
			result = 3;
		} else {
			n = n->next();
			result = itr->next();
		}
	}

	Delete(itr);
	PRINT_TEST_RESULTS(!result);
	return result;
}

void bintree_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (!test_BinInitializer()) p++;
	else f++;

	if (!test_BinInsert()) p++;
	else f++;
	
	if (!test_BinMax()) p++;
	else f++;

	if (!test_BinMin()) p++;
	else f++;

	if (!test_BinNodeKnowsItsLeaf()) p++;
	else f++;

	if (!test_BSTDelete()) p++;
	else f++;

	if (!test_RemovingRoot()) p++;
	else f++;

	if (!test_RemovingAll()) p++;
	else f++;

	if (!test_ReturningLeafValues()) p++;
	else f++;

	if (!test_BinTreeCount()) p++;
	else f++;

	if (!test_ReplacingBinNodes()) p++;
	else f++;

	if (!test_SearchingBinTree()) p++;
	else f++;

	if (!test_Iterations()) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // BINTREE_TESTS_HPP

