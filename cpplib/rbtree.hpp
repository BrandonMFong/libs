/**
 * author: Brando
 * date: 8/19/22
 */

#ifndef RBTREE_HPP
#define RBTREE_HPP

#include "delete.hpp"
#include "accessorspecifiers.hpp"
#include <iostream>
#include "bintree.hpp"

/**
 * Red Black Binary Tree
 *
 * Left most node is the least value comparison
 */
template <typename T, typename S = int> class RBTree : public BinTree<T,S> {
PRIVATE:
	class RBNode : public BinTree<T,S>::BinNode {
	PUBLIC:
		RBNode() : BinTree<T,S>::BinNode() {
			this->color = 'r';
		}

		virtual ~RBNode() {

		}

		virtual RBNode * grandParent() override {
			return (RBNode *) this->BinTree<T,S>::BinNode::grandParent();
		}
	
		virtual RBNode * pibling() override {
			return (RBNode *) this->BinTree<T,S>::BinNode::pibling();
		}
	
		virtual RBNode * sibling() override {
			return (RBNode *) this->BinTree<T,S>::BinNode::sibling();
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

		char color; // 'r' = red, 'b' = black
	};

PUBLIC:

	RBTree() : BinTree<T,S>() {
	}

	virtual ~RBTree() {
	}
	
	RBNode * root() override { return (RBNode *) this->BinTree<T,S>::root(); }
	void setRoot(RBNode * node) { this->BinTree<T,S>::setRoot(node); }
	
	virtual int insert(T obj) {
		RBNode * newNode = (RBNode *) this->createNode();
		newNode->obj = obj;

		if (this->root()) {
			return this->insert(newNode, this->root());
		} else {
			newNode->color = 'b';
			this->setRoot(newNode);
			this->root()->location = this->rootAddr();
			this->_count++;
			return 0;
		}
	}

PRIVATE:

	virtual void * createNode() {
		return new RBNode;
	}
	
	static bool isNodeBlack(RBNode * node) {
		if (!node) return true;
		else return node->color == 'b';
	}

	static bool isNodeRed(RBNode * node) {
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
	int insert(RBNode * newNode, RBNode * parent) {
		int result = this->BinTree<T,S>::insert(newNode, parent);
		if (result == 0) result = this->balance(newNode);
		return result;
	}

	/**
	 * Recursively balances tree
	 */
	int balance(RBNode * node) {
		RBNode * tmp = NULL;
		
		if (!node) {
			return 4;
		} else if (RBTree<T>::isNodeBlack(node)) {
			return 0;	
		} else if (RBTree<T>::isNodeBlack((RBNode *) node->parent)) {
			return 0;
		} else if (RBTree<T>::isNodeRed((RBNode *) node->parent)) {
			// Involves rotations
			if (RBTree<T>::isNodeBlack(node->pibling())) {
				return this->rotate(node);
			} else if (RBTree<T>::isNodeRed(node->pibling())) {
				if (node->parent) {
					((RBNode *) node->parent)->color = 'b';
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
	static unsigned char rotationCase(RBNode * node) {
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
	int rotate(RBNode * node) {
		int result = 0;
		RBNode * parent = (RBNode *) node->parent;
		RBNode * grandparent = node->grandParent();
		RBNode * tmp = NULL;
		const unsigned char ll = 1;
		const unsigned char lr = 2;
		const unsigned char rr = 3;
		const unsigned char rl = 4;
		unsigned char rcase = RBTree<T>::rotationCase(node);
		RBNode ** newLocation = NULL;
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
					tmp = (RBNode *) parent->right;
					newLocation = (RBNode **) &parent->right;
				} else {
					tmp = (RBNode *) parent->left;
					newLocation = (RBNode **) &parent->left;
				}

				// Do the replacement
				grandparent->parent = parent;
				*newLocation = grandparent;
				grandparent->location = (typename BinTree<T,S>::BinNode **) newLocation;

				// Figuring out new spot for the node
				// grandparent is replacing
				if (tmp) {
					tmp->parent = grandparent;
					if (rcase == ll) {
						newLocation = (RBNode **) &grandparent->left;
					} else {
						newLocation = (RBNode **) &grandparent->right;
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
					tmp = (RBNode *) node->left;
					newLocation = (RBNode **) &node->left;
				} else {
					tmp = (RBNode *) node->right;
					newLocation = (RBNode **) &node->right;
				}

				// Do the replacement
				parent->parent = (typename BinTree<T,S>::BinNode *) node;
				*newLocation = parent;
				parent->location = (typename BinTree<T,S>::BinNode **) newLocation;

				// Find the new spot for the node
				// parent is replacing
				if (tmp) {
					tmp->parent = parent;
					if (rcase == lr) {
						newLocation = (RBNode **) &parent->right;
					} else {
						newLocation =(RBNode **) &parent->left;
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
					tmp->location = (typename BinTree<T,S>::BinNode **) newLocation;
				}
			}
		}

		// rl and lr needs to handle the rr or ll respectively
		if ((result == 0) && ((rcase == lr) || (rcase == rl))) {
			result = this->rotate(parent);
		}

		return result;
	}
};

#endif // RBTREE_HPP

