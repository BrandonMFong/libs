/**
 * author: Brando
 * date: 9/7/22
 */

#ifndef BINTREE_HPP
#define BINTREE_HPP

#include "accessorspecifiers.hpp"
#include "delete.hpp"
#include <iostream>
#include "list.hpp"

/**
 * Standard Binary Tree
 *
 * Left most node is the least value comparison
 */
template <typename T, typename S = int> class BinTree {
PROTECTED:
	class BinNode {
		friend class BinTree<T,S>;

		T object() {
			return this->obj;
		}

		PROTECTED:
		BinNode() {
			this->obj = 0;
			this->_left = 0;
			this->_right = 0;
			this->parent = 0;
			this->location = 0;
		}

		virtual ~BinNode() {
			this->_left = 0;
			this->_right = 0;
			this->parent = 0;
			this->location = 0;
		}

		// Returns parent of parent
		// Null if no parent
		virtual BinNode * grandParent() {
			if (this->parent) return this->parent->parent;
			return NULL;
		}

		// Returns parent's sibling
		virtual BinNode * pibling() {
			BinNode * gp = NULL;
			
			if ((gp = this->grandParent()) != NULL) {
				if (gp->_left == this->parent) return gp->_right;
				else return gp->_left;
			}

			return NULL;
		}

		// Returns our sibling
		virtual BinNode * sibling() {
			switch (this->childType()) {
				case 0:
					return NULL;
				case -1:
					return this->parent->_right;
				case 1:
					return this->parent->_left;
				default:
					return NULL;
			}
		}

		virtual unsigned char childCount() {
			if (this->_left && this->_right) {
				return 2;
			} else if (this->_left || this->_right) {
				return 1;
			} else return 0;
		}

		// True if we have no childred
		virtual bool isLeaf() {
			return this->childCount() == 0;
		}

		// Is root if we have no parent
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
				if (this->parent->_left == this) return -1;
				else if (this->parent->_right == this) return 1;
				else return 0;
			}
		}

		// Returns number of levels we are from root
		int level() {
			return this->level(this->parent, 0);
		}

		virtual void print() {
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

		/**
		 * This node's children will not be exchanged.  The replacement's
		 * children will be retained. Additionally, the replacement's
		 * parent will no longer have any reference to node
		 *
		 * Use insert logic to place the children in correctly
		 */
		virtual int replaceWithNode(BinNode * replacement) {
			int result = 0;

			// Make parent forget about replacement node
			if (replacement) {
				if (replacement->parent) {
					switch (replacement->childType()) {
					case -1:
						replacement->parent->setLeft(NULL);
						break;
					case 1:
						replacement->parent->setRight(NULL);
						break;
					default:
						break;
					}
				}
			}

			if (this->isRoot()) {
				*this->location = replacement;
				if (replacement) {
					replacement->location = this->location;
					replacement->parent = NULL;
				}
			} else {
				switch (this->childType()) {
				case -1:
					this->parent->setLeft(replacement);
					break;
				case 1:
					this->parent->setRight(replacement);
					break;
				default:
					result = 2;
					break;
				}
			}

			if (result == 0) {
				this->parent = NULL;
				this->location = NULL;
			}

			return result;
		}

		// Makes shallow copy of us
		virtual BinNode * clone() {
			return new BinNode(*this);
		}

		// Sets left's parent if nonull
		virtual void setLeft(BinNode * left) {
			this->_left = left;
			if (this->_left) {
				this->_left->parent = this;
				this->_left->location = &this->_left;
			}
		}
	
		// Sets right's parent if nonull
		virtual void setRight(BinNode * right) {
			this->_right = right;
			if (this->_right) {
				this->_right->parent = this;
				this->_right->location = &this->_right;
			}
		}

		virtual BinNode * left() { return this->_left; }
		BinNode ** leftAddr() { return &this->_left; }

		virtual BinNode * right() { return this->_right; }
		BinNode ** rightAddr() { return &this->_right; }

		// Wondering if I want to make accessors for this
		BinNode * parent;

		/**
		 * Holds the address of where the node is in memory
		 */
		BinNode ** location;

		// Holds object
		T obj;

		PRIVATE:

		// Child node pointers
		BinNode * _left;
		BinNode * _right;
	
		// recursively executes until we reach root	
		int level(BinNode * node, int level) {
			if (node) return this->level(node->parent, ++level);
			else return level;
		}
	};

PUBLIC:

	BinTree() {
		this->_root = NULL;
		this->_compare = NULL;
		this->_count = 0;
	}

	virtual ~BinTree() {
		this->removeAll();
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
	virtual void print() {
		std::cout << std::endl;
		this->print(this->root());
		std::cout << std::endl;
	}

	// Returns object from the right most leaf
	T max() {
		BinNode * node = this->maxNode(this->root());
		return node ? node->obj : 0;
	}

	// Returns object from the left most leaf
	T min() {
		BinNode * node = this->minNode(this->root());
		return node ? node->obj : 0;
	}

	/**
	 * Removes the node that has the object
	 */
	virtual int remove(T obj) {
		int result = 0;
		BinNode * node = this->getNodeForObject(obj, this->root());
		if (node) {
			result = this->removeNode(node); // we do not need to know who replaced node
			
			// Delete node
			Delete(node);
		} else result = 1;
	
		if (result == 0) {
			this->_count--;
		}

		return result;
	}

	/**
	 * Deletes everything in the tree
	 */
	int removeAll() {
		int result = this->removeAll(this->root());
		if (result == 0) this->_root = NULL;
		return result;
	}

	/**
	 * Makes a list of all leaf node values
	 *
	 * Returns nonzero upon error
	 */
	int leafValues(List<T> * list) {
		List<BinNode *> leafNodes;
		int result = this->locateLeafNodes(&leafNodes, this->root());

		if (result == 0) {
			typename List<BinNode *>::Node * listNode = leafNodes.first();
			do {
				list->add(listNode->object()->obj);
			} while ((listNode = listNode->next()));
		}

		return result;
	}

PROTECTED:

	/// Root accessors
	BinNode ** rootAddr() { return (BinNode **) &this->_root; }
	void setRoot(BinNode * node) { this->_root = node; }
	
	// Derived classes can override
	virtual BinNode * root() { return (BinNode *) this->_root; }

	/**
	 * Holds size of tree
	 */	
	S _count;

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
			return this->getNodeForObject(obj, node->_left);
		case 1:
			return this->getNodeForObject(obj, node->_right);
		default:
			return NULL;
		}
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
					newLocation = &parent->_left;
					break;
				case 1:
					newLocation = &parent->_right;
					break;
				default:
					return 3;
			}
		}

		// Continue traversing
		if (!this->canNewNodeTakeNewLocation(newLocation)) return this->insert(newNode, *newLocation);
		else {
			newNode->parent = parent;
			newNode->location = newLocation;
			*newLocation = newNode;

			this->_count++;
		}

		return 0;
	}

	/**
	 * Tests newLocation for a BinNode.  We want to make sure a new node
	 * can live in this memory location.
	 *
	 * Created this function to allow base classes to override functionality and
	 * possibly prepare the newLocation for the BinNode
	 */
	virtual bool canNewNodeTakeNewLocation(BinNode ** newLocation) {
		return *newLocation == NULL;
	}

	/**
	 * Replaces node with its child
	 *
	 * We assume node only has one kid
	 */
	virtual int replaceNodeWithTheOnlyChild(BinNode * node) {
		int result = 0;
		BinNode * rep = NULL;

		if (node->childCount() != 1) {
			result = 3;
		}

		if (result == 0) {
			if (node->left()) {
				rep = node->left();
				node->setLeft(NULL);
			} else if (node->right()) {
				rep = node->right();
				node->setRight(NULL);
			} else {
				result = 2;
			}
		}

		// We do not need to worry about exchanging children
		// since node only has 1 child
		if (result == 0) {
			result = node->replaceWithNode(rep);
		}

		return result;
	}

	/**
	 * Performs standard BST Deletion
	 */
	int removeNode(BinNode * node) {
		int result = 0;
		unsigned char childCount = 0;

		if (node == NULL) {
			result = 1;
		} else {
			childCount = node->childCount();
		} 
	
		if (result == 0) {	
			if (childCount == 0) {
				// Right is null anyways
				result = node->replaceWithNode(node->left());
			} else if (childCount == 1) {
				result = this->replaceNodeWithTheOnlyChild(node);

			// replaces with the successor
			} else {
				// Find the replacement for node
				// should be a leaf
				BinNode * max = this->maxNode(node->left());

				// repNode will be used to put max node's data in
				// then we will put repNode in node
				BinNode * repNode = node->clone();
			
				// give max data to repNode
				repNode->obj = max->obj;

				result = node->BinNode::replaceWithNode(repNode);

				if (result == 0) {
					if (node->left() != repNode) {
						repNode->setLeft(node->left());
					}
					node->setLeft(NULL);

					if (node->right() != repNode) {
						repNode->setRight(node->right());
					}
					node->setRight(NULL);
				
					result = this->removeNode(max);
				}

				if (result == 0) {
					Delete(max);
				}
			}
		}

		return result;
	}

	virtual void print(BinNode * node) {
		if (node->right()) this->print(node->right());

		node->print();

		if (node->left()) this->print(node->left());
	}

	/// Returns the leaf node	
	virtual BinNode * maxNode(BinNode * node) {
		if (!node) return NULL;
		else if (node->right()) return this->maxNode(node->right());
		else return node;
	}

	/// Returns the leaf node	
	virtual BinNode * minNode(BinNode * node) {
		if (!node) return NULL;
		else if (node->left()) return this->minNode(node->left());
		else return node;
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
	 * Traverses through tree to remove everything
	 */
	int removeAll(BinNode * node) {
		int result = 0;

		// Just leave function if there is no node
		if (!node) return 0;

		if (node->left()) {
			result = this->removeAll(node->left());
		}

		if (result == 0) {
			if (node->right()) {
				result = this->removeAll(node->right());
			}
		}

		if (result == 0) {
			Delete(node);
			this->_count--;
		}

		return result;
	}

	/**
	 * Puts leaf nodes in outList
	 */
	virtual int locateLeafNodes(List<BinNode *> * outList, BinNode * node) {
		int result = 0;

		if (!node) return 1;
		else if (node->childCount() > 0) {
			if (node->left()) result = this->locateLeafNodes(outList, node->left());

			if (result == 0)
				if (node->right()) result = this->locateLeafNodes(outList, node->right());

			return result;
		} else {
			result = outList->add(node);
			return result;
		}
	}

	/**
	 * a1 == a2 -> 0
	 * a1 < a2 -> -1
	 * a1 > a2 -> 1
	 */
	int (* _compare) (T a1, T a2);

	/**
	 * The tip of the tree
	 */
	void * _root;
};

#endif // BINTREE_HPP

