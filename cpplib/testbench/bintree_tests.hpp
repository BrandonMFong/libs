/**
 * author: Brando
 * date: 9/7/22
 */

#ifndef BINTREE_TESTS_HPP
#define BINTREE_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include "../bintree.hpp"
#include "cpplib_tests.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
		n1.left = &n2;
		n1.right = &n3;

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
	t.insert(8);
	t.insert(7);
	t.insert(9);
	t.insert(1);
	t.insert(3);

	result = t.remove(3);

	if (result == 0) {
		result = t.remove(10);
	}
	
	if (result == 0) {
		result = t.remove(8);
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

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // BINTREE_TESTS_HPP

