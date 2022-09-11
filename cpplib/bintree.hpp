/**
 * author: Brando
 * date: 9/7/22
 */

#ifndef BINTREE_HPP
#define BINTREE_HPP

#include "delete.hpp"
#include "accessorspecifiers.hpp"
#include <iostream>

/**
 * Standard Binary Tree
 *
 * Left most node is the least value comparison
 */
template <typename T, typename S = int> class BinTree {
PRIVATE:
	class BinNode {
	PUBLIC:
		BinNode() {
			this->left = 0;
			this->right = 0;
			this->parent = 0;
			this->location = 0;
		}

		virtual ~BinNode() {
			this->left = 0;
			this->right = 0;
			this->parent = 0;
			this->location = 0;
		}

		virtual BinNode * grandParent() {
			if (this->parent) return this->parent->parent;
			return NULL;
		}

		virtual BinNode * pibling() {
			BinNode * gp = NULL;
			
			if ((gp = this->grandParent()) != NULL) {
				if (gp->left == this->parent) return gp->right;
				else return gp->left;
			}

			return NULL;
		}

		virtual BinNode * sibling() {
			switch (this->childType()) {
				case 0:
					return NULL;
				case -1:
					return this->parent->right;
				case 1:
					return this->parent->left;
				default:
					return NULL;
			}
		}

		bool isLeaf() {
			return !this->left && !this->right;
		}

		bool isRoot() {
			return (this->parent == NULL);
		}
	
		/**
		 * Whether node is left or right child
		 *
		 * Type:
		 * 	- (-1) : left
		 * 	- (0) : none (may be root)
		 * 	- (1) : right
		 */
		char childType() {
			if (!this->parent) return 0;
			else {
				if (this->parent->left == this) return -1;
				else if (this->parent->right == this) return 1;
				else return 0;
			}
		}

		int level() {
			return this->level(this->parent, 0);
		}

		void print() {
			int lvl = this->level();

			for (int i = 1; i <= lvl; i++) {
				if (i == lvl) {
					if (!this->isRoot()) std::cout << " |";
					std::cout << "----";
				} else {
					if (!this->isRoot()) {
						if (this->childType() != 0) std::cout << " |";
						else std::cout << "  ";
					}
					std::cout << "    ";
				}
			}
			
			std::cout << "[" << this->obj << "]" << std::endl;
		}	

		T obj;
		BinNode * left;
		BinNode * right;
		BinNode * parent;

		/**
		 * Holds the address of where the node is in memory
		 */
		BinNode ** location;

		PRIVATE:
		
		int level(BinNode * node, int level) {
			if (node) return this->level(node->parent, ++level);
			else return level;
		}
	};

PUBLIC:

	BinTree() {
		this->_root = NULL;
		this->_compare = NULL;
	}

	virtual ~BinTree() {
		this->removeAll();
	}

	// Root accessors
	virtual BinNode * root() { return (BinNode *) this->_root; }
	BinNode ** rootAddr() { return (BinNode **) &this->_root; }
	virtual void setRoot(BinNode * node) { this->_root = node; }

	/**
	 * Optional
	 *
	 * if not set, we will compare a and b literally
	 *
	 * Please set your own comparison if a and b are 
	 * some other object
	 */
	void setCompareCallback(int (* cb) (T a, T b)) {
		this->_compare = cb;
	}

	/**
	 * Inserts object into tree
	 */
	int insert(T obj) {
		BinNode * newNode = (BinNode *) this->createNode();
		newNode->obj = obj;

		if (this->root()) {
			return this->insert(newNode, this->root());
		} else {
			newNode->location = this->rootAddr();
			this->setRoot(newNode);
			this->_count++;
			return 0;
		}
	}

	S count() { return this->_count; }

	/**
	 * Sees if obj is inside our tree start from root
	 */
	bool contains(T obj) {
		return this->getNodeForObject(obj, this->root()) != NULL;
	}

	/**
	 * Prints tree with root at the left most positions and 
	 * the leafs at the right most, start from the top 
	 * the right leaves
	 */
	void print() {
		std::cout << std::endl;
		this->print(this->root());
		std::cout << std::endl;
	}

	T max() {
		BinNode * node = this->maxNode(this->root());
		return node ? node->obj : 0;
	}

	T min() {
		BinNode * node = this->minNode(this->root());
		return node ? node->obj : 0;
	}

	int remove(T obj) {
		BinNode * node = this->getNodeForObject(obj, this->root());

		return node ? this->removeNode(node) : 1;
	}

	/**
	 * Deletes everything in the tree
	 */
	int removeAll() {
		int result = this->removeAll(this->root());
		if (result == 0) this->_root = NULL;
		return result;
	}

PROTECTED:

	/**
	 * Creates BinNode
	 *
	 * Derived classes must override to submit their
	 * own node
	 */
	virtual void * createNode() {
		return new BinNode;
	}
	
	/**
	 * Inserts newNode into parent's children
	 *
	 * newNode should already have an obj set. Otherwise there will
	 * be undefined behavior
	 *
	 * Sets newNode's location field
	 */
	virtual int insert(BinNode * newNode, BinNode * parent) {
		BinNode ** newLocation = NULL;

		if (!newNode) return 1;
		else if (!parent) return 2;
		else {
			switch (this->runCompare(newNode->obj, parent->obj)) {
				case 0:
				case -1:
					newLocation = &parent->left;
					break;
				case 1:
					newLocation = &parent->right;
					break;
				default:
					return 3;
			}
		}

		// Continue traversing
		if (*newLocation) return this->insert(newNode, *newLocation);
		else {
			newNode->parent = parent;
			newNode->location = newLocation;
			*newLocation = newNode;

			this->_count++;
		}

		return 0;
	}

PRIVATE:

	/**
	 * If the _compare function pointer was not set, then 
	 * we will default by comparing its literal value
	 *
	 * 0xFFFFFFFF returned if comparison errored
	 */
	int runCompare(T a, T b) {
		if (this->_compare) return this->_compare(a, b);
		else {
			if (a == b) return 0;
			else if (a < b) return -1;
			else if (a > b) return 1;
			else return ~0;
		}
	}

	/**
	 * Returns nonnull pointer to BinNode
	 *
	 * Returns NULL if node with obj could not
	 * be found
	 */
	BinNode * getNodeForObject(T obj, BinNode * node) {
		if (!node) return NULL;
		
		switch (this->runCompare(obj, node->obj)) {
		case 0:
			return node;
		case -1:
			return this->getNodeForObject(obj, node->left);
		case 1:
			return this->getNodeForObject(obj, node->right);
		default:
			return NULL;
		}
	}

	void print(BinNode * node) {
		if (node->right) this->print(node->right);

		node->print();

		if (node->left) this->print(node->left);
			
	}

	BinNode * maxNode(BinNode * node) {
		if (!node) return NULL;
		else if (node->right) return this->maxNode(node->right);
		else return node;
	}
	
	BinNode * minNode(BinNode * node) {
		if (!node) return NULL;
		else if (node->left) return this->minNode(node->left);
		else return node;
	}

	/**
	 * Performs standard BST Deletion
	 *
	 * cases:
	 * 	- 	Is leaf. Just delete the leaf
	 * 	- 	Has two children. Copy contents of the inorder successor 
	 * 		to the node and delete the inorder successor. In this particular 
	 * 		case, inorder successor can be obtained by finding the 
	 * 		minimum value in the right child of the node
	 * 	- 	Has one child. replace node with child
	 */
	int removeNode(BinNode * node) {
		int result = 0;

		if (node == NULL) {
			result = 1;
		} else if (node->isLeaf()) {
			// erase traces of node at its location
			*node->location = NULL;
		} else if (node->left && node->right) {
			// Find the replacement for node
			// should be a leaf
			BinNode * min = this->minNode(node->right);

			// Take min out of its location
			*min->location = NULL;

			// Put min in node's location
			min->location = node->location;
			*node->location = min;
			min->parent = node->parent;

			// Make sure node's children know
			// the new parent
			min->left = node->left;
			if (min->left) min->left->parent = min;
			min->right = node->right;
			if (min->right) min->right->parent = min;
		} else {
			if (node->left) {
				node->left->location = node->location;
				*node->location = node->left;
				node->left->parent = node->parent;
			} else {
				node->right->location = node->location;
				*node->location = node->right;
				node->right->parent = node->parent;
			}
		}

		if (result == 0) {
			Delete(node);
			this->_count--;
		}

		return result;
	}

	/**
	 * Traverses through tree to remove everything
	 */
	int removeAll(BinNode * node) {
		int result = 0;

		// Just leave function if there is no node
		if (!node) return 0;

		if (node->left) {
			result = this->removeAll(node->left);
			node->left = NULL;
		}

		if (result == 0) {
			if (node->right) {
				result = this->removeAll(node->right);
				node->right = NULL;
			}
		}

		if (result == 0) {
			Delete(node);
			this->_count--;
		}

		return result;
	}

	/**
	 * a1 == a2 -> 0
	 * a1 < a2 -> -1
	 * a1 > a2 -> 1
	 */
	int (* _compare) (T a1, T a2);

	/**
	 * Holds size of tree
	 */	
	S _count;

	/**
	 * The tip of the tree
	 */
	void * _root;
};

#endif // BINTREE_HPP

