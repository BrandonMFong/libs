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
		BinNode * root = (BinNode *) this->_root;
		Delete(root);
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
		return this->contains(obj, this->root());
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
		return this->max(this->root());
	}

	T min() {
		return this->min(this->root());
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
	 * Checks if node has object obj. If not then it will keep traversing
	 */
	bool contains(T obj, BinNode * node) {
		if (!node) { 
			return false;
		}
		
		switch (this->runCompare(obj, node->obj)) {
		case 0:
			return true;
		case -1:
			return this->contains(obj, node->left);
		case 1:
			return this->contains(obj, node->right);
		default:
			return false;
		}
	}

	void print(BinNode * node) {
		if (node->right) this->print(node->right);

		node->print();

		if (node->left) this->print(node->left);
			
	}

	T max(BinNode * node) {
		if (!node) return 0;
		else if (node->right) return this->max(node->right);
		else return node->obj;
	}
	
	T min(BinNode * node) {
		if (!node) return 0;
		else if (node->left) return this->min(node->left);
		else return node->obj;
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

