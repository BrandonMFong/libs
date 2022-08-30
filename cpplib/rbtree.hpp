/**
 * author: Brando
 * date: 8/19/22
 */

#include "delete.hpp"
#include "accessorspecifiers.hpp"

/**
 * Red Black Binary Tree
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
		if (this->_root) {
			return this->insert(&this->_root, NULL, obj);
		} else {
			this->_root = new Node<T>;
			this->_root->obj = obj;
			this->_root->color = 'b';
			this->_count++;
			return 0;
		}
	}

	S count() { return this->_count; }

PRIVATE:

	template <typename N> class Node {
	PUBLIC:
		Node() {
			this->left = 0;
			this->right = 0;
			this->parent = 0;
			this->color = 'r';
		}

		virtual ~Node() {

		}

		Node<N> * grandParent() {
			if (this->parent) return this->parent->parent;
			return NULL;
		}

		Node<N> * pibling() {
			Node<N> * gp = NULL;
			
			if ((gp = this->grandParent()) != NULL) {
				if (gp->left == this->parent) return gp->right;
				else return gp->left;
			}

			return NULL;
		}

		Node<N> * sibling() {
			switch (this->childType()) {
				case 0:
					return NULL;
				case -1:
					return this->parent->right;
				case 1:
					return this->parent->left;
			}
		}

		bool isRoot() {
			return this->parent == NULL;
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

		N obj;
		Node<N> * left;
		Node<N> * right;
		Node<N> * parent;
		char color; // 'r' = red, 'b' = black
	};

	Node<T> * _root;

	static bool isNodeBlack(Node<T> * node) {
		if (!node) return true;
		else return node->color == 'b';
	}

	static bool isNodeRed(Node<T> * node) {
		if (!node) return false;
		else return node->color == 'r';
	}

	/**
	 * Recursively runs through node tree to find 
	 * a place for obj.  Will rebalance after obj
	 * is set into hierarchy
	 */
	int insert(Node<T> ** node, Node<T> * parent, T obj) {
		if (!node) return 1; // null ref
		else if (*node) { // traverse through hierarchy
			Node<T> ** n = NULL;
			switch (this->runCompare((*node)->obj, obj)) {
			case 0:
			case -1:
				n = &(*node)->left;
				break;
			case 1:
				n = &(*node)->right;
				break;
			default:
				return -1; // unknown error
			}

			return this->insert(n, *node, obj);
		} else { // insert
			Node<T> * n = new Node<T>;
			n->obj = obj;
			n->parent = parent;
			*node = n;
			this->_count++;

			return RBTree<T>::balance(n);
		}
	}

	/**
	 * Recursively balances tree
	 */
	static int balance(Node<T> * node) {
		Node<T> * tmp = NULL;
		
		if (!node) {
			return 4;
		} else if (RBTree<T>::isNodeBlack(node)) {
			return 0;	
		} else if (RBTree<T>::isNodeBlack(node->parent)) {
			return 0;
		} else if (RBTree<T>::isNodeRed(node->parent)) {
			// Involves rotations
			if (RBTree<T>::isNodeBlack(node->pibling())) {
				return RBTree<T>::rotate(node);
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

				return RBTree<T>::balance(tmp);
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
	static unsigned char rotationCase(Node<T> * node) {
		if (!node) return 0;
		else if (RBTree<T>::isNodeBlack(node)) return 0;
		else if (node->isRoot()) return 0;
		else if (node->childType() == 0) return 0;
		else {
			switch (node->parent->childType()) {
				case -1:
					return node->childType() == -1 ? 1 : 2;
				case 1:
					return node->childType() == 1 ? 3 : 4;
				default:
					return 0;
			}
		}	
	}

	/**
	 * Rotates the node, parent, pibling, and grandparent
	 */
	static int rotate(Node<T> * node) {
		Node<T> * parent = node->parent;
		Node<T> * grandparent = node->grandParent();
		Node<T> * pibling = node->pibling();
		unsigned char rcase = RBTree<T>::rotationCase(node);

		switch (rcase) {
			case 1: // ll
				break;
			case 2: // lr
				break;
			case 3: // rr
				break;
			case 4: // rl
				break;
			default:
				break;
		}

		return 0;
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
			else if (a < b) return 1;
			else if (a > b) return -1;
			else return ~0;
		}
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

