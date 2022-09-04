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

static int test_InsertingLRCase() {
	int result = 0;

	RBTree<int> i;

	result = i.insert(9);
	if (result) printf("Inserting error: %d\n", result);

	if (result == 0) {
		result = i.insert(3);
		if (result) printf("Inserting error: %d\n", result);
	}
	
	if (result == 0) {
		result = i.insert(7);
		if (result) printf("Inserting error: %d\n", result);
	}
	
	if (result == 0) {
		result = i.insert(1);
		if (result) printf("Inserting error: %d\n", result);
	}
	
	if (result == 0) {
		result = i.insert(2);
		if (result) printf("Inserting error: %d\n", result);
	}

	PRINT_TEST_RESULTS(!result);

	return result;
}

static int test_InsertingLLCase() {
	int result = 0;

	RBTree<int> i;

	result = i.insert(5);
	if (result) printf("Inserting error: %d\n", result);

	if (result == 0) {
		result = i.insert(4);
		if (result) printf("Inserting error: %d\n", result);
	}
	
	if (result == 0) {
		result = i.insert(3);
		if (result) printf("Inserting error: %d\n", result);
	}
	
	if (result == 0) {
		result = i.insert(2);
		if (result) printf("Inserting error: %d\n", result);
	}
	
	if (result == 0) {
		result = i.insert(1);
		if (result) printf("Inserting error: %d\n", result);
	}

	PRINT_TEST_RESULTS(!result);

	return result;
}

static int test_InsertingRRCase() {
	int result = 0;

	RBTree<int> i;

	result = i.insert(1);
	if (result) printf("Inserting error: %d\n", result);

	if (result == 0) {
		result = i.insert(2);
		if (result) printf("Inserting error: %d\n", result);
	}
	
	if (result == 0) {
		result = i.insert(3);
		if (result) printf("Inserting error: %d\n", result);
	}
	
	if (result == 0) {
		result = i.insert(4);
		if (result) printf("Inserting error: %d\n", result);
	}
	
	if (result == 0) {
		result = i.insert(5);
		if (result) printf("Inserting error: %d\n", result);
	}

	PRINT_TEST_RESULTS(!result);

	return result;
}

static int test_InsertingRLCase() {
	int result = 0;

	RBTree<int> i;

	result = i.insert(1);
	if (result) printf("Inserting error: %d\n", result);

	if (result == 0) {
		result = i.insert(6);
		if (result) printf("Inserting error: %d\n", result);
	}
	
	if (result == 0) {
		result = i.insert(2);
		if (result) printf("Inserting error: %d\n", result);
	}
	
	if (result == 0) {
		result = i.insert(8);
		if (result) printf("Inserting error: %d\n", result);
	}
	
	if (result == 0) {
		result = i.insert(7);
		if (result) printf("Inserting error: %d\n", result);
	}

	PRINT_TEST_RESULTS(!result);

	return result;
}

static int test_AccessingNodeHierarchy() {
	int result = 0;

	RBTree<int>::Node * n1;
	RBTree<int>::Node * n2;
	RBTree<int>::Node * n3;
	RBTree<int>::Node * n4;

	n1 = new RBTree<int>::Node;
	n2 = new RBTree<int>::Node;
	n3 = new RBTree<int>::Node;
	n4 = new RBTree<int>::Node;

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

	RBTree<int>::Node * n1 = 0;

	if (!RBTree<int>::isNodeBlack(n1)) {
		result = 1;
		printf("Should be black\n");
	}

	if (result == 0) {
		n1 = new RBTree<int>::Node;

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
	
	RBTree<int>::Node * n1;
	RBTree<int>::Node * n2;
	RBTree<int>::Node * n3;
	RBTree<int>::Node * n4;

	n1 = new RBTree<int>::Node;
	n2 = new RBTree<int>::Node;
	n3 = new RBTree<int>::Node;
	n4 = new RBTree<int>::Node;

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

int test_NodeLevels() {
	int result = 0;
	
	RBTree<int>::Node * n1;
	RBTree<int>::Node * n2;
	RBTree<int>::Node * n3;
	RBTree<int>::Node * n4;

	n1 = new RBTree<int>::Node;
	n2 = new RBTree<int>::Node;
	n3 = new RBTree<int>::Node;
	n4 = new RBTree<int>::Node;

	n4->parent = n2;
	n3->parent = n2;
	n2->parent = n1;

	n1->right = n4;
	n1->left = n2;
	n2->left = n3;

	if (n1->level() != 0) {
		printf("Level should be 0, but it is %d\n", n1->level());
		result = 1;
	} else if (n2->level() != 1) {
		printf("Level should be 1, but it is %d\n", n2->level());
		result = 2;
	} else if (n3->level() != 2) {
		printf("Level should be 2, but it is %d\n", n3->level());
		result = 3;
	} else if (n4->level() != 2) {
		printf("Level should be 2, but it is %d\n", n4->level());
		result = 4;
	}
	
	Delete(n1);
	Delete(n2);
	Delete(n3);
	Delete(n4);

	PRINT_TEST_RESULTS(!result);
	return result;
}

template <typename T>
class RBTreeChecker : public RBTree<T> {
PUBLIC:

	// true if passed 
    // All leaf nodes are black nodes.
	bool audit() {
		if (!this->rootIsColoredBlack()) {
			printf("Root is not black\n");
			return false;
		}

		if (!this->everyNodeHasAColor(this->_root)) {
			printf("Not every node has a color\n");
			return false;
		}

		if (!this->testColorPatterns(this->_root)) {
			printf("Found two adjacent red nodes\n");
			return false;
		}

		if (!this->checkPathsToLeaves()) {
			printf("Paths to leaves error\n");
			return false;
		}

		return true;
	}

    // The root of the tree is always black.
	bool rootIsColoredBlack() {
		return this->_root->color == 'b';
	}

	// Every node has a color either red or black.
	bool everyNodeHasAColor(RBTree<T>::Node * node) {
		bool res = true;
		if (node->left) res = this->everyNodeHasAColor(node->left);

		if (res)
			if (node->right) res = this->everyNodeHasAColor(node->right);

		if (res) res = (node->color == 'b') || (node->color == 'r');

		return res;
	}

    // There are no two adjacent red nodes (A red node cannot have a red parent or red child).
	bool testColorPatterns(RBTree<T>::Node * node) {
		bool res = true;

		if (node->parent) {
			if (node->parent->color == 'r') {
				res = node->color != 'r';
			}
		}

		if (res) 
			if (node->left) res = this->testColorPatterns(node->left);

		if (res)
			if (node->right) res = this->testColorPatterns(node->right);

		return res;
	}

    // Every path from a node (including root) to any of its descendants NULL nodes has the same number of black nodes.
	bool checkPathsToLeaves() {
		// true if there is any count
		// assuming the tester has inserting things into this tree
		if (!this->count()) return false;
		return this->countBlackNodesOnPathToLeaves(this->_root);
	}

	// Returns number black nodes there are on the way to 
	// the leaves, including param node
	//
	// returns 0 on error
	int countBlackNodesOnPathToLeaves(RBTree<T>::Node * node) {
		int lc = 0, rc = 0;

		if (node->left) lc = this->countBlackNodesOnPathToLeaves(node->left);
		else lc++;

		if (node->right) rc = this->countBlackNodesOnPathToLeaves(node->right);
		else rc++;
		
		if (!lc && !rc) return 0; 
		else if (lc != rc) return 0;
		else return RBTree<T>::isNodeBlack(node) ? ++lc : lc;
	}
};

int test_RBTreeChecker() {
	int result = 0;
	int max = 50; // max nodes
	
	RBTreeChecker<int> t;

	while (max) {
		int val = rand() % 100;

		result = t.insert(val);
		if (result) {
			printf("error: %d\n", result);
			break;
		}

		max--;
	}

	if (result == 0) {
		if (!t.audit()) {
			result = 1;
		}
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_RBTreeMax() {
	int result = 0;
	int max = 50; // max nodes
	int maxValue = 100;
	
	RBTreeChecker<int> t;

	while (max) {
		int val = rand() % maxValue;

		result = t.insert(val);
		if (result) {
			printf("error: %d\n", result);
			break;
		}

		max--;
	}

	if (result == 0) {
		result = t.insert(maxValue + 1);
	}

	if (result == 0) {
		if (t.max() != (maxValue + 1)) {
			result = 1;
			printf("Max should be %d but is %d\n", maxValue + 1, t.max());
		}
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_RBTreeMin() {
	int result = 0;
	int max = 50; // max nodes
	int minValue = 10;
	
	RBTreeChecker<int> t;

	while (max) {
		int val = (rand() % 100) + minValue;

		result = t.insert(val);
		if (result) {
			printf("error: %d\n", result);
			break;
		}

		max--;
	}

	if (result == 0) {
		result = t.insert(minValue - 1);
	}

	if (result == 0) {
		if (t.min() != (minValue - 1)) {
			result = 1;
			printf("Min should be %d but is %d\n", minValue - 1, t.min());
		}
	}


	PRINT_TEST_RESULTS(!result);
	return result;
}

void rbtree_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (!test_RBInitializer()) p++;
	else f++;

	if (!test_AccessingNodeHierarchy()) p++;
	else f++;

	if (!test_NodeColors()) p++;
	else f++;

	if (!test_RotationCases()) p++;
	else f++;

	if (!test_NodeLevels()) p++;
	else f++;

	if (!test_InsertingLLCase()) p++;
	else f++;

	if (!test_InsertingLRCase()) p++;
	else f++;

	if (!test_InsertingRRCase()) p++;
	else f++;

	if (!test_InsertingRLCase()) p++;
	else f++;

	if (!test_RBTreeChecker()) p++;
	else f++;

	if (!test_RBTreeMax()) p++;
	else f++;

	if (!test_RBTreeMin()) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // RBTREE_TESTS_HPP

