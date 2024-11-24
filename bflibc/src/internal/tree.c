/**
 * author: brando
 * date: 11/22/24
 *
 * https://www.geeksforgeeks.org/insertion-in-an-avl-tree/
 */

#include "tree.h"
#include "free.h"
#include "../bfmath.h"
#include <stdio.h>

BFTreeNode * BFTreeNodeCreate() {
	BFTreeNode * res = (BFTreeNode *) malloc(sizeof(BFTreeNode));
	res->left = NULL;
	res->right = NULL;
	res->object = NULL;
	res->height = 1;
	return res;
}

void BFTreeNodeRelease(BFTreeNode * node) {
	BFFree(node);
}

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

BFTreeNode * BFTreeNodeInsert(
	BFTreeNode * node,
	BFTreeNodeObject object,
	int (*compare)(BFTreeNodeObject a, BFTreeNodeObject b),
	int * error
) {
	// 1.  Perform the normal BST insertion
	if (node == NULL) {
		//return newNode;
		BFTreeNode * node = BFTreeNodeCreate();
		node->object = object;
		return node;
	}

	if (compare(object, node->object) < 0) {
		node->left = BFTreeNodeInsert(node->left, object, compare, error);
	} else if (compare(object, node->object) > 0) {
		node->right = BFTreeNodeInsert(node->right, object, compare, error);
	} else { // Equal keys are not allowed in BST
		*error = -1;
		return node;
	}

	// 2. Update height of this ancestor node
	node->height = 1 + BFMathMax(
		BFTreeNodeHeight(node->left),
		BFTreeNodeHeight(node->right));

	// 3. Get the balance factor of this ancestor
	// node to check whether this node became
	// unbalanced
	int balance = BFTreeNodeGetBalance(node);

	// If this node becomes unbalanced, then
	// there are 4 cases

	// Left Left Case
	if (balance > 1 && compare(object, node->left->object) < 0) {
		return BFTreeNodeRightRotate(node);
	}

	// Right Right Case
	if (balance < -1 && compare(object, node->right->object) > 0) {
		return BFTreeNodeLeftRotate(node);
	}

	// Left Right Case
	if (balance > 1 && compare(object, node->left->object) > 0) {
		node->left = BFTreeNodeLeftRotate(node->left);
		return BFTreeNodeRightRotate(node);
	}

	// Right Left Case
	if (balance < -1 && compare(object, node->right->object) < 0) {
		node->right = BFTreeNodeRightRotate(node->right);
		return BFTreeNodeLeftRotate(node);
	}

	// return the (unchanged) node pointer
	return node;
}

/* Given a non-empty binary search tree, return the
   node with minimum key value found in that tree.
   Note that the entire tree does not need to be
   searched. */
BFTreeNode * BFTreeNodeMinValueNode(BFTreeNode * node) {
	BFTreeNode * current = node;

	/* loop down to find the leftmost leaf */
	while (current->left != NULL) {
		current = current->left;
	}

	return current;
}

// Recursive function to delete a node with given key
// from subtree with given root. It returns root of
// the modified subtree.
BFTreeNode * BFTreeNodeRemove(
	BFTreeNode * root,
	BFTreeNodeObject object,
	int (*compare)(BFTreeNodeObject a, BFTreeNodeObject b)
) {
	// STEP 1: PERFORM STANDARD BST DELETE

	if (root == NULL) {
		return root;
	}

	// If the key to be deleted is smaller than the
	// root's key, then it lies in left subtree
	if (compare(object, root->object) < 0) {
		root->left = BFTreeNodeRemove(root->left, object, compare);
	// If the key to be deleted is greater than the
	// root's key, then it lies in right subtree
	} else if (compare(object, root->object) > 0) {
		root->right = BFTreeNodeRemove(root->right, object, compare);
	// if key is same as root's key, then This is
	// the node to be deleted
	} else {
		// node with only one child or no child
		if (root->left == NULL || root->right == NULL) {
			BFTreeNode *temp = root->left ? root->left : root->right;

			// No child case
			if (temp == NULL) {
				temp = root;
				root = NULL;
			} else { // One child case
				*root = *temp; // Copy the contents of
			}

			// the non-empty child
			BFTreeNodeRelease(temp);
		} else {
			// node with two children: Get the inorder
			// successor (smallest in the right subtree)
			BFTreeNode * temp = BFTreeNodeMinValueNode(root->right);

			// Copy the inorder successor's data to this node
			root->object = temp->object;
				

			// Delete the inorder successor
			root->right = BFTreeNodeRemove(root->right, temp->object, compare);
		}
	}

	// If the tree had only one node then return
	if (root == NULL) {
		return root;
	}

	// STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
	root->height = 1 + BFMathMax(BFTreeNodeHeight(root->left),
		BFTreeNodeHeight(root->right));

	// STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
	// check whether this node became unbalanced)
	int balance = BFTreeNodeGetBalance(root);

	// If this node becomes unbalanced, then there are 4 cases

	// Left Left Case
	if (balance > 1 && BFTreeNodeGetBalance(root->left) >= 0) {
		return BFTreeNodeRightRotate(root);
	}

	// Left Right Case
	if (balance > 1 && BFTreeNodeGetBalance(root->left) < 0) {
		root->left =  BFTreeNodeLeftRotate(root->left);
		return BFTreeNodeRightRotate(root);
	}

	// Right Right Case
	if (balance < -1 && BFTreeNodeGetBalance(root->right) <= 0) {
		return BFTreeNodeLeftRotate(root);
	}

	// Right Left Case
	if (balance < -1 && BFTreeNodeGetBalance(root->right) > 0) {
		root->right = BFTreeNodeRightRotate(root->right);
		return BFTreeNodeLeftRotate(root);
	}

	return root;
}

bool BFTreeNodeSearch(
	BFTreeNode * node,
	BFTreeNodeObject obj,
	int (*compare)(BFTreeNodeObject a, BFTreeNodeObject b)
) {
	if (!node) {
		return false;
	} else if (!obj) {
		return false;
	}

	int comp = compare(node->object, obj);
	if (comp == 0) {
		return true;
	} else if (comp < 0) {
		return BFTreeNodeSearch(node->right, obj, compare);
	} else {
		return BFTreeNodeSearch(node->left, obj, compare);
	}
}

