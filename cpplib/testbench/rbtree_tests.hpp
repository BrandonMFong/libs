/**
 * author: Brando
 * date: 6/30/22
 */

#ifndef RBTREE_TESTS_HPP
#define RBTREE_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include "../rbtree.hpp"
#include "cpplib_tests.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int test_RBInitializer() {
	PRINT_TEST_RESULTS(1);

	RBTree<int> i;
	RBTree<double> d;
	RBTree<float> f;
	RBTree<char> c;
	
	RBTree<int> * ii = new RBTree<int>;
	RBTree<double> * dd = new RBTree<double>;
	RBTree<float> * ff = new RBTree<float>;
	RBTree<char> * cc = new RBTree<char>;

	Delete(ii);
	Delete(dd);
	Delete(ff);
	Delete(cc);

	return 0;
}

static int test_Inserting() {
	int result = 0;

	RBTree<int> i;

	result = i.insert(1);
	if (result) printf("Inserting 1 error: %d\n", result);

	if (result == 0) {
		result = i.insert(2);
		if (result) printf("Inserting 1 error: %d\n", result);
	}
	
	PRINT_TEST_RESULTS(!result);

	return result;
}

static int test_AccessingNodeHierarchy() {
	int result = 0;

	RBTree<int>::Node<int> * n1;
	RBTree<int>::Node<int> * n2;
	RBTree<int>::Node<int> * n3;
	RBTree<int>::Node<int> * n4;

	n1 = new RBTree<int>::Node<int>;
	n2 = new RBTree<int>::Node<int>;
	n3 = new RBTree<int>::Node<int>;
	n4 = new RBTree<int>::Node<int>;

	n3->parent = n2;
	n2->parent = n1;

	n1->right = n4;
	n1->left = n2;
	n2->left = n3;

	if (n3->grandParent() != n1) {
		result = 1;
		printf("Incorrect grandparent\n");
	} else if (n1->grandParent() != NULL) {
		result = 2;
		printf("grand parent should be null\n");
	} else if (n1->parent != NULL) {
		result = 3;
		printf("parent should be null\n");
	} else if (n3->pibling() != n4) {
		printf("Incorrect pibling\n");
		result = 4;
	} else if (n2->sibling() != n4) {
		printf("Incorrect sibling\n");
		result = 5;
	} else if (n1->sibling() != NULL) {
		printf("Should not have a sibling\n");
		result = 6;
	}

	Delete(n1);
	Delete(n2);
	Delete(n3);
	Delete(n4);

	PRINT_TEST_RESULTS(!result);

	return result;
}

int test_NodeColors() {
	int result = 0;

	RBTree<int>::Node<int> * n1 = 0;

	if (!RBTree<int>::isNodeBlack(n1)) {
		result = 1;
		printf("Should be black\n");
	}

	if (result == 0) {
		n1 = new RBTree<int>::Node<int>;

		n1->color = 'r';

		if (!RBTree<int>::isNodeRed(n1)) {
			result = 2;
			printf("Should be red\n");
		} else if (RBTree<int>::isNodeBlack(n1)) {
			result = 3;
			printf("Should not be black\n");
		}
	}

	if (result == 0) {
		n1->color = 'b';

		if (RBTree<int>::isNodeRed(n1)) {
			result = 2;
			printf("Should not be red\n");
		} else if (!RBTree<int>::isNodeBlack(n1)) {
			result = 3;
			printf("Should be black\n");
		}
	}

	Delete(n1);

	PRINT_TEST_RESULTS(!result);

	return result;
}

int test_RotationCases() {
	int result = 0;
	
	RBTree<int>::Node<int> * n1;
	RBTree<int>::Node<int> * n2;
	RBTree<int>::Node<int> * n3;
	RBTree<int>::Node<int> * n4;

	n1 = new RBTree<int>::Node<int>;
	n2 = new RBTree<int>::Node<int>;
	n3 = new RBTree<int>::Node<int>;
	n4 = new RBTree<int>::Node<int>;

	n3->parent = n2;
	n4->parent = n2;
	n2->parent = n1;

	n1->left = n2;
	n2->left = n3;
	n2->right = n4;

	if (RBTree<int>::rotationCase(n4) != 2) {
		printf("Case should be 2 but is %d\n", RBTree<int>::rotationCase(n4));
		result = 1;
	} else if (RBTree<int>::rotationCase(n3) != 1) {
		printf("Case should be 1 but is %d\n", RBTree<int>::rotationCase(n4));
		result = 2;
	}

	if (result == 0) {
		n1->left = n2;
		if (RBTree<int>::rotationCase(n4) != 2) {
			printf("Case should be 2 but is %d\n", RBTree<int>::rotationCase(n4));
			result = 1;
		} else if (RBTree<int>::rotationCase(n3) != 1) {
			printf("Case should be 1 but is %d\n", RBTree<int>::rotationCase(n4));
			result = 2;
		}
	}
	
	Delete(n1);
	Delete(n2);
	Delete(n3);
	Delete(n4);

	PRINT_TEST_RESULTS(!result);

	return result;
}

void rbtree_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (!test_RBInitializer()) p++;
	else f++;

	if (!test_Inserting()) p++;
	else f++;

	if (!test_AccessingNodeHierarchy()) p++;
	else f++;

	if (!test_NodeColors()) p++;
	else f++;

	if (!test_RotationCases()) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // RBTREE_TESTS_HPP

