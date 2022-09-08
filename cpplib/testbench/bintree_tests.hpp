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

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // BINTREE_TESTS_HPP

