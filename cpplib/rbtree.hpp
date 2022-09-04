/**
 * author: Brando
 * date: 8/19/22
 */

#include "delete.hpp"
#include "accessorspecifiers.hpp"
#include <iostream>

/**
 * Red Black Binary Tree
 *
 * Left most node is the least value comparison
 */
template <typename T, typename S = int> class RBTree {
PUBLIC:
	RBTree() {
		this->_root = NULL;
		this->_compare = NULL;
	}

	virtual ~RBTree() {
		Delete(this->_root);
	}

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
		Node * newNode = new Node;
		newNode->obj = obj;

		if (this->_root) {
			return this->insert(newNode, this->_root);
		} else {
			this->_root = newNode;
			this->_root->color = 'b';
			this->_root->location= &this->_root;
			this->_count++;
			return 0;
		}
	}

	S count() { return this->_count; }

	/**
	 * Sees if obj is inside our tree start from root
	 */
	bool contains(T obj) {
		return this->contains(obj, this->_root);
	}

	/**
	 * Prints tree with root at the left most positions and 
	 * the leafs at the right most, start from the top 
	 * the right leaves
	 */
	void print() {
		std::cout << std::endl;
		this->print(this->_root);
		std::cout << std::endl;
	}

	T max() {
		return this->max(this->_root);
	}

	T min() {
		return this->min(this->_root);
	}

PRIVATE:

	class Node {
	PUBLIC:
		Node() {
			this->left = 0;
			this->right = 0;
			this->parent = 0;
			this->color = 'r';
			this->location = 0;
		}

		virtual ~Node() {

		}

		Node * grandParent() {
			if (this->parent) return this->parent->parent;
			return NULL;
		}

		Node * pibling() {
			Node * gp = NULL;
			
			if ((gp = this->grandParent()) != NULL) {
				if (gp->left == this->parent) return gp->right;
				else return gp->left;
			}

			return NULL;
		}

		Node * sibling() {
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
			
			if (this->color == 'r') std::cout << "\033[0;31m";
			std::cout << "[" << this->obj << "]";
			if (this->color == 'r') std::cout << "\033[0m";
			std::cout << std::endl;
		}	

		T obj;
		Node * left;
		Node * right;
		Node * parent;
		char color; // 'r' = red, 'b' = black

		/**
		 * Holds the address of where the node is in memory
		 */
		Node ** location;
		PRIVATE:
		
		int level(Node * node, int level) {
			if (node) return this->level(node->parent, ++level);
			else return level;
		}
	};

	Node * _root;

	static bool isNodeBlack(Node * node) {
		if (!node) return true;
		else return node->color == 'b';
	}

	static bool isNodeRed(Node * node) {
		if (!node) return false;
		else return node->color == 'r';
	}

	/**
	 * Inserts newNode into parent's children
	 *
	 * newNode should already have an obj set. Otherwise there will
	 * be undefined behavior
	 *
	 * Sets newNode's location field
	 */
	int insert(Node * newNode, Node * parent) {
		Node ** newLocation = NULL;

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

		if (!newLocation) return 4;

		// Continue traversing
		else if (*newLocation) return this->insert(newNode, *newLocation);
		else {
			newNode->parent = parent;
			newNode->location = newLocation;
			*newLocation = newNode;

			this->_count++;

			return this->balance(newNode);
		}
	}

	/**
	 * Recursively balances tree
	 */
	int balance(Node * node) {
		Node * tmp = NULL;
		
		if (!node) {
			return 4;
		} else if (RBTree<T>::isNodeBlack(node)) {
			return 0;	
		} else if (RBTree<T>::isNodeBlack(node->parent)) {
			return 0;
		} else if (RBTree<T>::isNodeRed(node->parent)) {
			// Involves rotations
			if (RBTree<T>::isNodeBlack(node->pibling())) {
				return this->rotate(node);
			} else if (RBTree<T>::isNodeRed(node->pibling())) {
				if (node->parent) {
					node->parent->color = 'b';
				}

				if ((tmp = node->pibling()) != NULL) {
					tmp->color = 'b';
				}
				
				if ((tmp = node->grandParent()) != NULL) {
					if (!tmp->isRoot()) tmp->color = 'r';
				}

				return this->balance(tmp);
			} else {
				return 2;
			}
		} else {
			return 3;
		}
	}

	/**
	 * Returns the rotation case
	 * Cases:
	 * 	- 0: unknown
	 * 	- 1: ll
	 * 	- 2: lr
	 * 	- 3: rr
	 * 	- 4: rl
	 */
	static unsigned char rotationCase(Node * node) {
		if (!node) return 0;
		else if (RBTree<T>::isNodeBlack(node)) return 0;
		else if (node->isRoot()) return 0;
		else if (node->childType() == 0) return 0;
		else {
			switch (node->parent->childType()) {
				case -1:
					switch (node->childType()) {
						case -1:
							return 1;
						case 1:
							return 2;
						default:
							return 0;
					}
				case 1:
					switch (node->childType()) {
						case -1:
							return 4;
						case 1:
							return 3;
						default:
							return 0;
					}
				default:
					return 0;
			}
		}	
	}

	/**
	 * Rotates the node, parent, and grandparent
	 */
	int rotate(Node * node) {
		int result = 0;
		Node * parent = node->parent;
		Node * grandparent = node->grandParent();
		Node * tmp = NULL;
		const unsigned char ll = 1;
		const unsigned char lr = 2;
		const unsigned char rr = 3;
		const unsigned char rl = 4;
		unsigned char rcase = RBTree<T>::rotationCase(node);
		Node ** newLocation = NULL;
		char childType = 0;

		// Figure out who we are replacing	
		if ((rcase == ll) || (rcase ==  rr)) {
			childType = parent->childType();
			tmp = grandparent;
		} else if ((rcase == lr) || (rcase == rl)) {
			childType = node->childType();
			tmp = parent;
		}

		// tmp will be replaced by the node whose spot we are 
		// nulling out
		switch (childType) {
			case -1:
				tmp->left = NULL;
				break;
			case 1:
				tmp->right = NULL;
				break;
			default:
				result = 1;
				break;
		}
		
		if (result == 0) {
			tmp = NULL;
			if ((rcase == ll) || (rcase ==  rr)) {
				// Change colors
				parent->color = 'b';
				grandparent->color = 'r';

				// Put parent in grandparent's location
				*grandparent->location = parent;
				parent->location = grandparent->location;
				parent->parent = grandparent->parent; // should work if gp is root

				// Clear grandparent's position memory
				grandparent->parent = NULL;
				grandparent->location = NULL;

				// Find new spot for grandparent
				if (rcase == ll) {
					tmp = parent->right;
					newLocation = &parent->right;
				} else {
					tmp = parent->left;
					newLocation = &parent->left;
				}

				// Do the replacement
				grandparent->parent = parent;
				*newLocation = grandparent;
				grandparent->location = newLocation;

				// Figuring out new spot for the node
				// grandparent is replacing
				if (tmp) {
					tmp->parent = grandparent;
					if (rcase == ll) {
						newLocation = &grandparent->left;
					} else {
						newLocation = &grandparent->right;
					}
				}
			} else if ((rcase == lr) || (rcase == rl)) {
				// Put node in parent's position
				node->parent = parent->parent;
				node->location = parent->location;
				*node->location = node;
				parent->parent = NULL;
				parent->location = NULL;

				// Find where the parent will be next
				if (rcase == lr) {
					tmp = node->left;
					newLocation = &node->left;
				} else {
					tmp = node->right;
					newLocation = &node->right;
				}

				// Do the replacement
				parent->parent = node;
				*newLocation = parent;
				parent->location = newLocation;

				// Find the new spot for the node
				// parent is replacing
				if (tmp) {
					tmp->parent = parent;
					if (rcase == lr) {
						newLocation = &parent->right;
					} else {
						newLocation = &parent->left;
					}
				}
			
			// no rotation case
			} else {
				result = 2;
			}
		}

		// If the rotation orphaned a node (tmp), then
		// we need to put it in the newLocation
		if (result == 0) {
			if (tmp) {
				if (*newLocation) result = this->insert(tmp, *newLocation);
				else {
					*newLocation = tmp;
					tmp->location = newLocation;
				}
			}
		}

		// rl and lr needs to handle the rr or ll respectively
		if ((result == 0) && ((rcase == lr) || (rcase == rl))) {
			result = this->rotate(parent);
		}

		return result;
	}

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
	bool contains(T obj, Node * node) {
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

	void print(Node * node) {
		if (node->right) this->print(node->right);

		node->print();

		if (node->left) this->print(node->left);
			
	}

	T max(RBTree<T>::Node * node) {
		if (!node) return 0;
		else if (node->right) return this->max(node->right);
		else return node->obj;
	}
	
	T min(RBTree<T>::Node * node) {
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
};

