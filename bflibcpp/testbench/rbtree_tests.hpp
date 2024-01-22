/**
 * author: Brando
 * date: 6/30/22
 */

#ifndef RBTREE_TESTS_HPP
#define RBTREE_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include <rbtree.hpp>
#include "cpplib_tests.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list.hpp>
#include <string.hpp>
#include <math.h>

using namespace BF;

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

	RBTree<int>::RBNodeNonnull * n1;
	RBTree<int>::RBNodeNonnull * n2;
	RBTree<int>::RBNodeNonnull * n3;
	RBTree<int>::RBNodeNonnull * n4;

	n1 = new RBTree<int>::RBNodeNonnull;
	n2 = new RBTree<int>::RBNodeNonnull;
	n3 = new RBTree<int>::RBNodeNonnull;
	n4 = new RBTree<int>::RBNodeNonnull;

	n3->_parent = n2;
	n2->_parent = n1;

	n1->setRight(n4);
	n1->setLeft(n2);
	n2->setLeft(n3);

	if (n3->grandParent() != n1) {
		result = 1;
		printf("Incorrect grandparent\n");
	} else if (n1->grandParent() != NULL) {
		result = 2;
		printf("grand parent should be null\n");
	} else if (n1->_parent != NULL) {
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

	RBTree<int>::RBNodeNonnull * n1 = 0;

	if (!RBTree<int>::isNodeBlack(n1)) {
		result = 1;
		printf("Should be black\n");
	}

	if (result == 0) {
		n1 = new RBTree<int>::RBNodeNonnull;

		n1->setColor(kRBTreeNodeColorRed);

		if (!RBTree<int>::isNodeRed(n1)) {
			result = 2;
			printf("Should be red\n");
		} else if (RBTree<int>::isNodeBlack(n1)) {
			result = 3;
			printf("Should not be black\n");
		}
	}

	if (result == 0) {
		n1->setColor(kRBTreeNodeColorBlack);

		if (RBTree<int>::isNodeRed(n1)) {
			result = 2;
			printf("Should not be red\n");
		} else if (!RBTree<int>::isNodeBlack(n1)) {
			result = 3;
			printf("Should be black\n");
		}
	}

	if (result == 0) {
		if (n1->colorCount() != 1) {
			result = 5;
			printf("Color count is %d\n", n1->colorCount());
			printf("Error %d\n", result);
		}
	}

	if (result == 0) {
		n1->setColorCount(2);
		if (n1->colorCount() != 2) {
			result = 6;
			printf("Color count is %d\n", n1->colorCount());
			printf("Error %d\n", result);
		}
	
	}

	Delete(n1);

	PRINT_TEST_RESULTS(!result);

	return result;
}

int test_RotationCases() {
	int result = 0;
	RBTree<int> t;

	RBTree<int>::RBNodeNonnull * n1;
	RBTree<int>::RBNodeNonnull * n2;
	RBTree<int>::RBNodeNonnull * n3;
	RBTree<int>::RBNodeNonnull * n4;

	n1 = new RBTree<int>::RBNodeNonnull;
	n2 = new RBTree<int>::RBNodeNonnull;
	n3 = new RBTree<int>::RBNodeNonnull;
	n4 = new RBTree<int>::RBNodeNonnull;

	n3->_parent = n2;
	n4->_parent = n2;
	n2->_parent = n1;

	n1->setLeft(n2);
	n2->setLeft(n3);
	n2->setRight(n4);

	if (t.rotationCase(n4) != 2) {
		printf("Case should be 2 but is %d\n", t.rotationCase(n4));
		result = 1;
	} else if (t.rotationCase(n3) != 1) {
		printf("Case should be 1 but is %d\n", t.rotationCase(n4));
		result = 2;
	}

	if (result == 0) {
		n1->setLeft(n2);
		if (t.rotationCase(n4) != 2) {
			printf("Case should be 2 but is %d\n", t.rotationCase(n4));
			result = 1;
		} else if (t.rotationCase(n3) != 1) {
			printf("Case should be 1 but is %d\n", t.rotationCase(n4));
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
	
	RBTree<int>::RBNodeNonnull * n1;
	RBTree<int>::RBNodeNonnull * n2;
	RBTree<int>::RBNodeNonnull * n3;
	RBTree<int>::RBNodeNonnull * n4;

	n1 = new RBTree<int>::RBNodeNonnull;
	n2 = new RBTree<int>::RBNodeNonnull;
	n3 = new RBTree<int>::RBNodeNonnull;
	n4 = new RBTree<int>::RBNodeNonnull;

	n1->setRight(n4);
	n1->setLeft(n2);
	n2->setLeft(n3);

	if (n1->level() != 0) {
		printf("Level should be 0, but it is %d\n", n1->level());
		result = 1;
	} else if (n2->level() != 1) {
		printf("Level should be 1, but it is %d\n", n2->level());
		result = 2;
	} else if (n3->level() != 2) {
		printf("Level should be 2, but it is %d\n", n3->level());
		result = 3;
	} else if (n4->level() != 1) {
		printf("Level should be 1, but it is %d\n", n4->level());
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

		if (!this->everyNodeHasAColor(this->root())) {
			printf("Not every node has a color\n");
			return false;
		}

		if (!this->testColorPatterns(this->root())) {
			printf("Found two adjacent red nodes\n");
			return false;
		}

		if (!this->checkPathsToLeaves()) {
			printf("Paths to leaves error\n");
			return false;
		}

		if (!this->everyLeafHasRBNodeNullObjects()) {
			printf("Leaf nodes don't all have valid null nodes\n");
			return false;
		}

		return true;
	}

    // The root of the tree is always black.
	bool rootIsColoredBlack() {
		return this->root()->color() == kRBTreeNodeColorBlack;
	}

	// Every node has a color either red or black.
	bool everyNodeHasAColor(typename RBTree<T>::RBNode * node) {
		bool res = true;
		if (node->left()) res = this->everyNodeHasAColor((typename RBTree<T>::RBNodeNonnull *) node->left());

		if (res)
			if (node->right()) res = this->everyNodeHasAColor((typename RBTree<T>::RBNodeNonnull *) node->right());

		if (res) res = (node->color() == kRBTreeNodeColorBlack) || (node->color() == kRBTreeNodeColorRed);

		return res;
	}

    // There are no two adjacent red nodes (A red node cannot have a red parent or red child).
	bool testColorPatterns(typename RBTree<T>::RBNode * node) {
		bool res = true;

		if (node->_parent) {
			if (((typename RBTree<T>::RBNodeNonnull *) node->_parent)->color() == kRBTreeNodeColorRed) {
				res = node->color() != kRBTreeNodeColorRed;

				if (!res) {
					std::cout << "Found error:" << std::endl;
					std::cout << "\tRed parent: " << node->_parent->_obj << std::endl;
					std::cout << "\tRed node: " << node->_obj << std::endl;
				}
			}
		}

		if (res) 
			if (node->left()) res = this->testColorPatterns((typename RBTree<T>::RBNodeNonnull *) node->left());

		if (res)
			if (node->right()) res = this->testColorPatterns((typename RBTree<T>::RBNodeNonnull *) node->right());

		return res;
	}

    // Every path from a node (including root) to any of its descendants NULL nodes has the same number of black nodes.
	bool checkPathsToLeaves() {
		// true if there is any count
		// assuming the tester has inserting things into this tree
		if (!this->count()) return false;
		return this->countBlackNodesOnPathToLeaves(this->root());
	}

	// Returns number black nodes there are on the way to 
	// the leaves, including param node
	//
	// returns 0 on error
	int countBlackNodesOnPathToLeaves(typename RBTree<T>::RBNode * node) {
		int lc = 0, rc = 0;

		if (node->left()) lc = this->countBlackNodesOnPathToLeaves((typename RBTree<T>::RBNodeNonnull *) node->left());

		if (node->right()) rc = this->countBlackNodesOnPathToLeaves((typename RBTree<T>::RBNodeNonnull *) node->right());
		
		if (lc != rc) {
			printf("Left count: %d, right count: %d\n", lc, rc);
			std::cout << "Node: " << node->_obj << std::endl;
			return 0;
		}
	
		return RBTree<T>::isNodeBlack(node) ? lc + node->colorCount() : lc;
	}

	bool everyLeafHasRBNodeNullObjects() {
		int result = 0;
		List<const typename BinTree<T>::BinNode *> nodes;
		result = this->locateLeafNodes(&nodes, this->root());

		if (result == 0) {
			typename List<const typename BinTree<T>::BinNode *>::Node * lnode = nodes.first();
			do {
				typename RBTree<T>::RBNodeNonnull * rbn = (typename RBTree<T>::RBNodeNonnull *) lnode->object();
				typename RBTree<T>::RBNodeNonnull * l = (typename RBTree<T>::RBNodeNonnull *) rbn->left();
				typename RBTree<T>::RBNodeNonnull * r = (typename RBTree<T>::RBNodeNonnull *) rbn->right();
				
				if (!l) {
					printf("left is null\n");
					result = 3;
				} else if (!r) {
					printf("right is null\n");
					result = 4;
				} else if (!l->isNull()) {
					result = 1;
				} else if (!r->isNull()) {
					result = 2;
				}

				if (result) {
					std::cout << "node: " << rbn->_obj << std::endl;
					std::cout << "parent: " << rbn->_parent->_obj << std::endl;
					std::cout << "error: " << result << std::endl;
				}
			} while ((lnode = lnode->next()) && !result);
		}

		return !result;
	}
};

int test_RBTreeChecker() {
	int result = 0;
	int max = 3; // max nodes
	
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
	
	RBTree<int> t;

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
	
	RBTree<int> t;

	while (max) {
		int val = (rand() % 100) + minValue;

		result = t.insert(val);
		if (result) {
			printf("error in insertion: %d\n", result);
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

/**
 * This will add values 0 to `addCount` and remove up to `removeCount` objects
 */
int test_RBRemove() {
	int result = 0;
	int addCount = 500;
	int removeCount = 25;

	RBTreeChecker<int> t;
	for (int i = 1; i <= addCount; i++) {
		t.insert(i);
	}

	// I use this for debugging an error pattern
#if 0
	/*
	Remove error: 150
	An issue occurred removing: 454
	Removed: 

	// Problems:
	[ 149  195  393  458  103  16  464  26  359  12  449 ]
	[ 383  359  284  181  479  472  48 ]
	[ 141  359  214  468  332  356  84  155  223  222  100  161  414 ]
	*/

	t.print(true);
	int object = 16;
	printf("Remove %d\n", object);
	result = t.remove(object);
	if (!result) {
		if (!t.audit()) {
			result = 1;
			printf("Audit failed\n");
		}
	}
	t.print(true);
#else
	List<int> removedObjects;
	srand(time(0));
	for (int i = 0; i < removeCount; i++) {
		int object = 0;
		do {
			object = (rand() % addCount) + 1;
		} while (removedObjects.contains(object));

		result = t.remove(object);
		if (!result) {
			if (!t.audit()) {
				result = 1;
				printf("Audit failed\n");
			}
		}

		if (result) {
			printf("Remove error: %d\n", result);
			printf("An issue occurred removing: %d\n", object);
			printf("Removed: \n");
			removedObjects.print();
			t.print(true);
			break;
		}

		removedObjects.add(object);
	}
#endif

	if (result) printf("Error %d\n", result);

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_RBTreeCount() {
	int result = 0;
	RBTree<int> t;
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

int test_SearchingRBTree() {
	int result = 0;
	const int maxVal = 2000;
	srand(time(0));

	int target = rand() % maxVal;
	RBTree<int> t;

	for (int i = 0; i < maxVal; i++) {
		if (i != target) {
			result = t.insert(i);

			if (result) break;
		}
	}

	if (!result) {
		result = t.insert(target);
	}

	if (!result) {
		bool found = false;
		const RBTree<int>::RBNode * n = t.root();
		do {
			int obj = n->object();
			if (target < obj) n = n->left();
			else if (target > obj) n = n->right();
			else found = true;
		} while (!found && !n->isNull());

		if (!found) {
			result = 2;
		}
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_insertingStrings(void) {
	UNIT_TEST_START;
	int result = 0;

	int numtests = 2 << 4;
	while (!result && numtests) {
		RBTree<String> t;
		char buf[4];
		for (int i = 0x61; i <= 0x7a; i++) {
			for (int j = 0x61; j <= 0x7a; j++) {
				for (int k = 0x61; k <= 0x7a; k++) {
					snprintf(buf, 4, "%c%c%c", i, j, k);
					String str = buf;
					t.insert(str);
				}
			}
		}
		int expsize = pow(0x7a - 0x60, 3);
		if (t.count() != expsize) result = 1;

		numtests--;
	}

	UNIT_TEST_END(!result, result);
	return result;
}

void rbtree_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	LAUNCH_TEST(test_RBInitializer,p, f);
	LAUNCH_TEST(test_AccessingNodeHierarchy,p, f);
	LAUNCH_TEST(test_NodeColors,p, f);
	LAUNCH_TEST(test_RotationCases,p, f);
	LAUNCH_TEST(test_NodeLevels,p, f);
	LAUNCH_TEST(test_InsertingLLCase,p, f);
	LAUNCH_TEST(test_InsertingLRCase,p, f);
	LAUNCH_TEST(test_InsertingRRCase,p, f);
	LAUNCH_TEST(test_InsertingRLCase,p, f);
	LAUNCH_TEST(test_RBTreeChecker,p, f);
	LAUNCH_TEST(test_RBTreeMax,p, f);
	LAUNCH_TEST(test_RBTreeMin,p, f);
	LAUNCH_TEST(test_RBRemove,p, f);
	LAUNCH_TEST(test_RBTreeCount,p, f);
	LAUNCH_TEST(test_SearchingRBTree,p, f);
	LAUNCH_TEST(test_insertingStrings,p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // RBTREE_TESTS_HPP

