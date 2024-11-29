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

_BFTreeNode * _BFTreeNodeCreate() {
	_BFTreeNode * res = (_BFTreeNode *) malloc(sizeof(_BFTreeNode));
	res->left = NULL;
	res->right = NULL;
	res->object = NULL;
	res->height = 1;
	return res;
}

void _BFTreeNodeRelease(
	_BFTreeNode * node,
	void (*release)(BFTreeObject object)
) {
	if (release) {
		release(node->object);
	}
	BFFree(node);
}

// A utility function to get the height of the tree
int _BFTreeNodeHeight(_BFTreeNode * node) {
    if (node == NULL)
        return 0;
    return node->height;
}

// A utility function to right rotate subtree rooted with y
// See the diagram given above.
_BFTreeNode * _BFTreeNodeRightRotate(_BFTreeNode * y) {
    _BFTreeNode *x = y->left;
    _BFTreeNode *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = BFMathMax(_BFTreeNodeHeight(y->left),
                    _BFTreeNodeHeight(y->right)) + 1;
    x->height = BFMathMax(_BFTreeNodeHeight(x->left),
                    _BFTreeNodeHeight(x->right)) + 1;

    // Return new root
    return x;
}

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
_BFTreeNode * _BFTreeNodeLeftRotate(_BFTreeNode * x) {
    _BFTreeNode *y = x->right;
    _BFTreeNode *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    //  Update heights
    x->height = BFMathMax(_BFTreeNodeHeight(x->left),   
                    _BFTreeNodeHeight(x->right)) + 1;
    y->height = BFMathMax(_BFTreeNodeHeight(y->left),
                    _BFTreeNodeHeight(y->right)) + 1;

    // Return new root
    return y;
}

// Get Balance factor of node N
int _BFTreeNodeGetBalance(_BFTreeNode * node) {
    if (node == NULL)
        return 0;
    return _BFTreeNodeHeight(node->left) - _BFTreeNodeHeight(node->right);
}

_BFTreeNode * _BFTreeNodeInsert(
	_BFTreeNode * node,
	BFTreeObject object,
	int (*compare)(BFTreeObject a, BFTreeObject b),
	int * error
) {
	// 1.  Perform the normal BST insertion
	if (node == NULL) {
		_BFTreeNode * node = _BFTreeNodeCreate();
		node->object = object;
		return node;
	}

	if (compare(object, node->object) < 0) {
		node->left = _BFTreeNodeInsert(
			node->left, object, compare, error
		);
	} else if (compare(object, node->object) > 0) {
		node->right = _BFTreeNodeInsert(
			node->right, object, compare, error
		);
	} else { // Equal keys are not allowed in BST
		if (error) *error = -1;
		return node;
	}

	// 2. Update height of this ancestor node
	node->height = 1 + BFMathMax(
		_BFTreeNodeHeight(node->left),
		_BFTreeNodeHeight(node->right));

	// 3. Get the balance factor of this ancestor
	// node to check whether this node became
	// unbalanced
	int balance = _BFTreeNodeGetBalance(node);

	// If this node becomes unbalanced, then
	// there are 4 cases

	// Left Left Case
	if (balance > 1 && compare(object, node->left->object) < 0) {
		return _BFTreeNodeRightRotate(node);
	}

	// Right Right Case
	if (balance < -1 && compare(object, node->right->object) > 0) {
		return _BFTreeNodeLeftRotate(node);
	}

	// Left Right Case
	if (balance > 1 && compare(object, node->left->object) > 0) {
		node->left = _BFTreeNodeLeftRotate(node->left);
		return _BFTreeNodeRightRotate(node);
	}

	// Right Left Case
	if (balance < -1 && compare(object, node->right->object) < 0) {
		node->right = _BFTreeNodeRightRotate(node->right);
		return _BFTreeNodeLeftRotate(node);
	}

	// return the (unchanged) node pointer
	return node;
}

/* Given a non-empty binary search tree, return the
   node with minimum key value found in that tree.
   Note that the entire tree does not need to be
   searched. */
_BFTreeNode * _BFTreeNodeMinValueNode(_BFTreeNode * node) {
	_BFTreeNode * current = node;

	/* loop down to find the leftmost leaf */
	while (current->left != NULL) {
		current = current->left;
	}

	return current;
}

// Recursive function to delete a node with given key
// from subtree with given root. It returns root of
// the modified subtree.
_BFTreeNode * _BFTreeNodeRemove(
	_BFTreeNode * root,
	BFTreeObject object,
	int (*compare)(BFTreeObject a, BFTreeObject b),
	void (*release)(BFTreeObject object)
) {
	// STEP 1: PERFORM STANDARD BST DELETE

	if (root == NULL) {
		return root;
	}

	// If the key to be deleted is smaller than the
	// root's key, then it lies in left subtree
	if (compare(object, root->object) < 0) {
		root->left = _BFTreeNodeRemove(
			root->left, object, compare, release
		);
	// If the key to be deleted is greater than the
	// root's key, then it lies in right subtree
	} else if (compare(object, root->object) > 0) {
		root->right = _BFTreeNodeRemove(
			root->right, object, compare, release
		);
	// if key is same as root's key, then This is
	// the node to be deleted
	} else {
		// node with only one child or no child
		if (root->left == NULL || root->right == NULL) {
			_BFTreeNode * temp = root->left ? root->left : root->right;

			// No child case
			if (temp == NULL) {
				temp = root;
				root = NULL;
			} else { // One child case
				BFTreeObject o = root->object;
				*root = *temp; // Copy the contents of
				temp->object = o;
			}
		
		/*	
			if (release) {
				release(object);
			}
			*/
	
			// the non-empty child
			_BFTreeNodeRelease(temp, release);
		} else {
			// node with two children: Get the inorder
			// successor (smallest in the right subtree)
			_BFTreeNode * temp = _BFTreeNodeMinValueNode(root->right);

			// Copy the inorder successor's data to this node
			root->object = temp->object;

			// Delete the inorder successor
			root->right = _BFTreeNodeRemove(
				root->right, temp->object, compare, NULL
			);
		}
	}

	// If the tree had only one node then return
	if (root == NULL) {
		return root;
	}

	// STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
	root->height = 1 + BFMathMax(_BFTreeNodeHeight(root->left),
		_BFTreeNodeHeight(root->right));

	// STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
	// check whether this node became unbalanced)
	int balance = _BFTreeNodeGetBalance(root);

	// If this node becomes unbalanced, then there are 4 cases

	// Left Left Case
	if (balance > 1 && _BFTreeNodeGetBalance(root->left) >= 0) {
		return _BFTreeNodeRightRotate(root);
	}

	// Left Right Case
	if (balance > 1 && _BFTreeNodeGetBalance(root->left) < 0) {
		root->left =  _BFTreeNodeLeftRotate(root->left);
		return _BFTreeNodeRightRotate(root);
	}

	// Right Right Case
	if (balance < -1 && _BFTreeNodeGetBalance(root->right) <= 0) {
		return _BFTreeNodeLeftRotate(root);
	}

	// Right Left Case
	if (balance < -1 && _BFTreeNodeGetBalance(root->right) > 0) {
		root->right = _BFTreeNodeRightRotate(root->right);
		return _BFTreeNodeLeftRotate(root);
	}

	return root;
}

bool _BFTreeNodeSearch(
	_BFTreeNode * node,
	BFTreeObject obj,
	int (*compare)(BFTreeObject a, BFTreeObject b)
) {
	if (!node) {
		return false;
	} else if (!obj) {
		return false;
	}

	int comp = compare(obj, node->object);
	if (comp == 0) {
		return true;
	} else if (comp < 0) {
		return _BFTreeNodeSearch(node->left, obj, compare);
	} else {
		return _BFTreeNodeSearch(node->right, obj, compare);
	}
}

