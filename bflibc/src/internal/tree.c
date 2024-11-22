/**
 * author: brando
 * date: 11/22/24
 *
 * https://www.geeksforgeeks.org/insertion-in-an-avl-tree/
 */

#include "tree.h"
#include "../bfmath.h"

// A utility function to get the height of the tree
int BFTreeNodeHeight(BFTreeNode * node) {
    if (node == NULL)
        return 0;
    return node->height;
}

// A utility function to right rotate subtree rooted with y
// See the diagram given above.
BFTreeNode * BFTreeNodeRightRotate(BFTreeNode * y) {
    BFTreeNode *x = y->left;
    BFTreeNode *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = BFMathMax(BFTreeNodeHeight(y->left),
                    BFTreeNodeHeight(y->right)) + 1;
    x->height = BFMathMax(BFTreeNodeHeight(x->left),
                    BFTreeNodeHeight(x->right)) + 1;

    // Return new root
    return x;
}

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
BFTreeNode * BFTreeNodeLeftRotate(BFTreeNode * x) {
    BFTreeNode *y = x->right;
    BFTreeNode *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    //  Update heights
    x->height = BFMathMax(BFTreeNodeHeight(x->left),   
                    BFTreeNodeHeight(x->right)) + 1;
    y->height = BFMathMax(BFTreeNodeHeight(y->left),
                    BFTreeNodeHeight(y->right)) + 1;

    // Return new root
    return y;
}

// Get Balance factor of node N
int BFTreeNodeGetBalance(BFTreeNode * node) {
    if (node == NULL)
        return 0;
    return BFTreeNodeHeight(node->left) - BFTreeNodeHeight(node->right);
}

BFTreeNode * BFTreeInsert(
	BFTreeNode * node,
	BFTreeNode * newNode,
	int (*compare)(BFTreeNodeObject a, BFTreeNodeObject b)
) {
	/* 1.  Perform the normal BST insertion */
	if (node == NULL) {
		return newNode;
	}

	if (compare(newNode->object, node->object) < 0) {
		node->left = BFTreeInsert(node->left, newNode, compare);
	} else if (compare(newNode->object, node->object) > 0) {
		node->right = BFTreeInsert(node->right, newNode, compare);
	} else { // Equal keys are not allowed in BST
		return node;
	}

	/* 2. Update height of this ancestor node */
	node->height = 1 + BFMathMax(
		BFTreeNodeHeight(node->left),
		BFTreeNodeHeight(node->right));

	/* 3. Get the balance factor of this ancestor
	node to check whether this node became
	unbalanced */
	int balance = BFTreeNodeGetBalance(node);

	// If this node becomes unbalanced, then
	// there are 4 cases

	// Left Left Case
	if (balance > 1 && compare(newNode->object, node->left->object) < 0) {
		return BFTreeNodeRightRotate(node);
	}

	// Right Right Case
	if (balance < -1 && compare(newNode->object, node->right->object) > 0) {
		return BFTreeNodeLeftRotate(node);
	}

	// Left Right Case
	if (balance > 1 && compare(newNode->object, node->left->object) > 0) {
		node->left = BFTreeNodeLeftRotate(node->left);
		return BFTreeNodeRightRotate(node);
	}

	// Right Left Case
	if (balance < -1 && compare(newNode->object, node->right->object) < 0) {
		node->right = BFTreeNodeRightRotate(node->right);
		return BFTreeNodeLeftRotate(node);
	}

	/* return the (unchanged) node pointer */
	return node;
}

