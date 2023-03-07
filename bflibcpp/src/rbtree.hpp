/**
 * author: Brando
 * date: 8/19/22
 */

#ifndef RBTREE_HPP
#define RBTREE_HPP

#include "accessorspecifiers.hpp"
#include "bintree.hpp"
#include "delete.hpp"
#include <iostream>

/// Node colors
#define kRBTreeNodeColorRed 0x00
#define kRBTreeNodeColorBlack 0x01

/**
 * The color should be stored in the least significant 4 bits of a byte
 */
#define kRBTreeNodeMaskColor 0x0F

/**
 * Color count will be in the most significant 4 bits of a byte
 */
#define kRBTreeNodeMaskColorCount 0xF0

namespace BF {

/**
 * Red Black Binary Tree
 *
 * Left most node is the least value comparison
 */
template <typename T, typename S = int> class RBTree : public BinTree<T,S> {
PUBLIC:
	// TODO: rename rbnode to node
	class RBNode : public BinTree<T,S>::BinNode {
		friend class RBTree<T,S>;

	PUBLIC:
		RBNode() : BinTree<T,S>::BinNode() {
			this->_colorSpace = 0;

			this->setColor(kRBTreeNodeColorRed);
			this->setColorCount(1);
		}

		virtual ~RBNode() {}

		virtual bool isNull() const { return false; }
		virtual const RBNode * left() const { return (const RBNode *) this->BinTree<T,S>::BinNode::left(); }
		virtual const RBNode * right() const { return (const RBNode *) this->BinTree<T,S>::BinNode::right(); }

	PROTECTED:

		virtual void printObject() const { std::cout << this->_obj; }

		virtual RBNode * left() { return (RBNode *) this->BinTree<T,S>::BinNode::left(); }
		virtual RBNode * right() { return (RBNode *) this->BinTree<T,S>::BinNode::right(); }

		virtual void setLeft(typename BinTree<T,S>::BinNode * left) { this->BinTree<T,S>::BinNode::setLeft(left); }
		virtual void setRight(typename BinTree<T,S>::BinNode * right) { this->BinTree<T,S>::BinNode::setRight(right); }

		void print() const {
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
			
			if (this->color() == kRBTreeNodeColorRed) std::cout << "\033[0;31m";

			for (int i = 0; i < this->colorCount(); i++) std::cout << "[";
			this->printObject();
			for (int i = 0; i < this->colorCount(); i++) std::cout << "]";

			if (this->color() == kRBTreeNodeColorRed) std::cout << "\033[0m";
			std::cout << std::endl;
		}
	
		virtual unsigned char childCount() const {
			RBNode * l = (RBNode *) this->left(), 
				   * r = (RBNode *) this->right();
		
			if (l->isNull() && r->isNull()) {
				return 0;
			} else if (!l->isNull() && !r->isNull()) {
				return 2;
			} else return 1;
		}

		virtual RBNode * clone() {
			return new RBNode(*this);
		}

		virtual RBNode * grandParent() {
			return (RBNode *) this->BinTree<T,S>::BinNode::grandParent();
		}
	
		virtual RBNode * pibling() {
			return (RBNode *) this->BinTree<T,S>::BinNode::pibling();
		}
	
		virtual RBNode * sibling() {
			return (RBNode *) this->BinTree<T,S>::BinNode::sibling();
		}

		char color() const {
			return this->_colorSpace & kRBTreeNodeMaskColor;
		}

		void setColor(char color) {
			this->_colorSpace = (this->_colorSpace & ~kRBTreeNodeMaskColor) | (color & kRBTreeNodeMaskColor);
		}

		unsigned char colorCount() const {
			return (this->_colorSpace & kRBTreeNodeMaskColorCount) >> 4;
		}

		void setColorCount(unsigned char count) {
			this->_colorSpace = (this->_colorSpace & ~kRBTreeNodeMaskColorCount) | ((count << 4) & kRBTreeNodeMaskColorCount);
		}

	PRIVATE:

		/**
		 * This will hold the color value in 0x0f and the color count in 0xf0
		 */		
		unsigned char _colorSpace;
	};

	/**
	 * Every node will start with a null node for their left and right children
	 */
	class RBNodeNull : public RBNode {
	PUBLIC:
		RBNodeNull() : RBNode() {
			this->setColor(kRBTreeNodeColorBlack);
		}

		virtual ~RBNodeNull() {}
		
		virtual bool isNull() const { return true; }

		virtual void printObject() const { std::cout << "_"; }
		
		virtual RBNodeNull * clone() {
			return new RBNodeNull(*this);
		}
	};

	class RBNodeNonnull : public RBNode {
	PUBLIC:
		RBNodeNonnull() : RBNode() {
			this->setColor(kRBTreeNodeColorRed);

			this->setLeft(new RBNodeNull);
			this->setRight(new RBNodeNull);
		}

		virtual ~RBNodeNonnull() {}

		// Identifies if RBNodeNonnull is a null type node
		virtual bool isNull() const { return false; }
		
		virtual RBNodeNonnull * clone() {
			return new RBNodeNonnull(*this);
		}

	PROTECTED:

		virtual void setLeft(typename BinTree<T,S>::BinNode * left) { 
			if (left) this->RBNode::setLeft(left);
			else {
				this->RBNode::setLeft(new RBNodeNull);
				this->left()->_location = this->leftAddr();
				this->left()->_parent = this;
			}
		}

		virtual void setRight(typename BinTree<T,S>::BinNode * right) {
			if (right) this->RBNode::setRight(right);
			else {
				this->RBNode::setRight(new RBNodeNull);
				this->right()->_location = this->rightAddr();
				this->right()->_parent = this;
			}
		}
	};
	
	class Iterator : public BinTree<T,S>::Iterator {
		friend class RBTree<T,S>;
	PROTECTED:
		Iterator() : BinTree<T,S>::Iterator() {}

		/**
		 * Sets stack to every node's left child
		 */
		int setCurrent(typename BinTree<T,S>::BinNode * bnode) {
			RBNode * node = (RBNode *) bnode;
			if (!node) return 1;
			else if (node->isNull()) return 0;
			else {
				int result = this->_st.push(node);

				if (!result && !node->left()->isNull()) 
					return this->setCurrent(node->left());
				else return result;
			}
		}
	};

	RBTree() : BinTree<T,S>() {}

	virtual ~RBTree() {}
	
	virtual const RBNode * root() const { return (const RBNode *) this->BinTree<T,S>::root(); }

	/**
	 * inserts object into tree
	 */
	virtual int insert(T obj) {
		RBNode * newNode = (RBNode *) this->createNode();
		newNode->_obj = obj;

		if (this->root()) {
			return this->insert(newNode, this->root());
		} else {
			newNode->setColor(kRBTreeNodeColorBlack);
			this->setRoot(newNode);
			this->root()->_location = this->rootAddr();
			this->setNodeLocation(this->root(), this->rootAddr());
			this->_count++;
			return 0;
		}
	}

	/**
	 * Removes object from tree
	 *
	 * refs: https://www.geeksforgeeks.org/red-black-tree-set-3-delete-2/
	 *
	 * notes:
	 * 	 When a black node is deleted and replaced by a black child, the child is marked as double black
	 */	
	virtual int remove(T obj) {
		RBNode * node = (RBNode *) this->getNodeForObject(obj, (typename BinTree<T,S>::BinNode *) this->root());

		if (!node) return 101;
		else return this->deleteNode(node);
	}

	virtual void print() {
		this->print(false);
	}

	/**
	 * param `printNullNodes`: If true, then null nodes will be shown
	 */
	virtual void print(bool printNullNodes) {
		std::cout << std::endl;
		this->print(this->root(), printNullNodes);
		std::cout << std::endl;
	}

	/**
	 * Removes node from tree and deletes the memory
	 *
	 * This is was made so that caller does not have to cast RBNode to BinNode
	 */
	int deleteNode(const RBNode * node) {
		return this->deleteNode((typename BinTree<T,S>::BinNode *) node);
	}

	virtual const RBNode * minNode() const {
		return (const RBNode *) this->minNode(this->root());
	}

	virtual const RBNode * maxNode() const {
		return (const RBNode *) this->maxNode(this->root());
	}

	virtual int createIterator(Iterator ** itr) {
		int result = 0;
		if (!itr) return 9;
		else {
			*itr = new RBTree<T,S>::Iterator;
			result = (*itr)->setCurrent(this->root());
		}
		return result;
	}

PRIVATE:
	
	virtual RBNode * root() { return (RBNode *) this->BinTree<T,S>::root(); }

	virtual void * createNode() {
		return new RBNodeNonnull;
	}
	
	/**
	 * Removes node from tree and deletes it from memory
	 */
	virtual int deleteNode(typename BinTree<T,S>::BinNode * bnode) {
		RBNode * node = (RBNode *) bnode;
		bool isRoot = node->isRoot();

		// The dbLocation will have the node we will need to interrogate if it
		// has a double black node
		RBNode * max = (RBNode *) this->maxNode(node->left());
		
		// If max has a child, it would only have a right child
		RBNode * dbNode = (RBNode *) max->left();
		if (max->isNull()) {
			dbNode = max;
		}
		
		int result = this->removeNode(node);

		if (result == 0) {
			// We should have at least a null node object
			if (!dbNode) result = 107;
			else {
				if (dbNode->colorCount() == 2) {
					result = this->balanceRemoval(dbNode);
				}
			}
		}

		// If we are deleting the root node then we need to make
		// sure the root remains black
		if (result == 0) {
			if (isRoot) {
				if (RBTree<T,S>::isNodeRed(this->root())) {
					this->root()->setColor(kRBTreeNodeColorBlack);
				}
			}
		}

		// Delete null nodes if any
		if (node->left())
			if (node->left()->isNull()) {
				RBNode * l = (RBNode *) node->left();
				Delete(l);
			}

		if (node->right())
			if ((node->right())->isNull()) {
				RBNode * r = (RBNode *) node->right();
				Delete(r);
			}

		// Delete node
		Delete(node);
	
		if (result == 0) {
			this->_count--;
		}

		return result;
	}

	static bool isNodeBlack(const RBNode * node) {
		if (!node) return true;
		else if (node->isNull()) return true;
		else return node->color() == kRBTreeNodeColorBlack;
	}

	static bool isNodeRed(const RBNode * node) {
		if (!node) return false;
		else if (node->isNull()) return false;
		else return node->color() == kRBTreeNodeColorRed;
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
		if (result == 0) result = this->balanceInsertion(newNode);
		return result;
	}
	
	virtual bool canNewNodeTakeNewLocation(typename BinTree<T,S>::BinNode ** newLocation) {
		RBNode ** rbLocation = (RBNode **) newLocation;

		if (!(*rbLocation)->isNull()) return false;
		else {
			Delete(*rbLocation);
			return true;
		}
	}

	/**
	 * Recursively balances tree for insertions
	 */
	int balanceInsertion(RBNode * node) {
		RBNode * tmp = NULL;
		
		if (!node) {
			return 110;
		} else if (RBTree<T>::isNodeBlack(node)) {
			return 0;	
		} else if (RBTree<T>::isNodeBlack((RBNode *) node->_parent)) {
			return 0;
		} else if (RBTree<T>::isNodeRed((RBNode *) node->_parent)) {
			// Involves rotations
			if (RBTree<T>::isNodeBlack(node->pibling())) {
				RBNode * parent = (RBNode *) node->_parent;
				unsigned char rcase = this->rotationCase(node);

				if ((rcase == ROTATION_CASE_RR) || (rcase == ROTATION_CASE_LL)) {
					if ((tmp = (RBNode *) node->_parent)) {
						tmp->setColor(kRBTreeNodeColorBlack);
					}

					if ((tmp = node->grandParent())) {
						tmp->setColor(kRBTreeNodeColorRed);
					}
				}

				int result = this->rotate(node, rcase);
				
				if (result == 0) {
					if ((rcase == ROTATION_CASE_RL) || (rcase == ROTATION_CASE_LR)) {
						if ((tmp = (RBNode *) parent->_parent)) {
							tmp->setColor(kRBTreeNodeColorBlack);
						}

						if ((tmp = parent->grandParent())) {
							tmp->setColor(kRBTreeNodeColorRed);
						}
					}

					// lr and rl need to handle ll and rr respectively after
					// their rotation case
					if (rcase == ROTATION_CASE_LR) {
						result = this->rotate(parent, ROTATION_CASE_LL);
					} else if (rcase == ROTATION_CASE_RL) {
						result = this->rotate(parent, ROTATION_CASE_RR);
					}
				}

				return result;

			} else if (RBTree<T>::isNodeRed(node->pibling())) {
				if (node->_parent) {
					((RBNode *) node->_parent)->setColor(kRBTreeNodeColorBlack);
				}

				if ((tmp = node->pibling()) != NULL) {
					tmp->setColor(kRBTreeNodeColorBlack);
				}
				
				if ((tmp = node->grandParent()) != NULL) {
					if (!tmp->isRoot()) tmp->setColor(kRBTreeNodeColorRed);
				}

				return this->balanceInsertion(tmp);
			} else {
				return 113;
			}
		} else {
			return 114;
		}
	}

	/**
	 * Balances trees specifically for remove operations
	 *
	 * param 'node': The node that replaced the removed node. We assume the colors have been set
	 *
	 * https://www.youtube.com/watch?v=CTvfzU_uNKE&list=LL&index=2
	 */
	int balanceRemoval(RBNode * node) {
		int result = 0;
		RBNode * sibling = node->sibling();
		RBNode * parent = (RBNode *) node->_parent;
		bool doCaseSix = false;

		if ((node->colorCount() < 2) && !RBTree<T>::isNodeBlack(node)) {
			result = 122;
		}

		if (result == 0) {
			// Case 1
			if (node->isRoot()) {
				node->setColorCount(1);

			// Case 3, 4, 5, 6
			} else if (RBTree<T>::isNodeBlack(sibling)) {
				unsigned char rcase = 0;
				RBNode * childOne = NULL, * childTwo = NULL;
				char sibChildType = sibling->childType();
				switch (sibChildType) {
				case -1:
					childOne = (RBNode *) sibling->right();
					childTwo = (RBNode *) sibling->left();
					break;
				case 1:
					childOne = (RBNode *) sibling->left();
					childTwo = (RBNode *) sibling->right();
					break;
				default:
					result = 124;
					break;
				}

				if (result == 0) {
					// Case 3, 5
					if (RBTree<T>::isNodeBlack(parent)) {
						// 5 (Will result in case 6)
						if (RBTree<T>::isNodeRed(childOne) && RBTree<T>::isNodeBlack(childTwo)) {
							switch (childOne->childType()) {
							case -1:
								rcase = ROTATION_CASE_RL;
								break;
							case 1:
								rcase = ROTATION_CASE_LR;
								break;
							default:
								break;
							}
							
							sibling->setColor(kRBTreeNodeColorRed);
							sibling->left()->setColor(kRBTreeNodeColorBlack);
							sibling->right()->setColor(kRBTreeNodeColorBlack);

							result = this->rotate(childOne, rcase);

							if (result == 0) {
								result = this->balanceRemoval(node);
							}

						// 6
						} else if (RBTree<T>::isNodeRed(childTwo)) {
							doCaseSix = true;
						// 3
						} else {
							node->setColorCount(1);
							parent->setColorCount(2);
							sibling->setColor(kRBTreeNodeColorRed);

							result = this->balanceRemoval(parent);
						}

					// Case 4, 6
					} else if (RBTree<T>::isNodeRed(parent)) {
						// Case 4
						if (RBTree<T>::isNodeBlack(childOne) && RBTree<T>::isNodeBlack(childTwo)) {
							sibling->setColor(kRBTreeNodeColorRed);
							parent->setColor(kRBTreeNodeColorBlack);
							node->setColorCount(1);

						// Case 6
						} else if (RBTree<T>::isNodeRed(childTwo)) {
							doCaseSix = true;
						} else {
							// We should never be in this case
							result = 150;
						}
					}
				}

				// Case 6
				if (!result && doCaseSix) {
					switch (childTwo->childType()) {
					case -1:
						rcase = ROTATION_CASE_LL;
						break;
					case 1:
						rcase = ROTATION_CASE_RR;
						break;
					default:
						break;
					}

					childTwo->setColor(kRBTreeNodeColorBlack);
					sibling->setColor(parent->color());
					parent->setColor(kRBTreeNodeColorBlack);
					node->setColorCount(1);

					result = this->rotate(childTwo, rcase);
				}

			// Case 2
			} else if (RBTree<T>::isNodeRed(sibling)) {
				if (RBTree<T>::isNodeBlack(sibling->left()) && RBTree<T>::isNodeBlack(sibling->right())) {
					unsigned char rcase = 0;
					RBNode * child = NULL;
					switch (sibling->childType()) {
					case -1:
						rcase = ROTATION_CASE_LL;
						child = sibling->left();
						break;
					case 1:
						rcase = ROTATION_CASE_RR;
						child = sibling->right();
						break;
					default:
						result = 123;
					}

					if (result == 0) {
						parent->setColor(kRBTreeNodeColorRed);
						sibling->setColor(kRBTreeNodeColorBlack);
						result = this->rotate((RBNode *) child, rcase);
					}

					if (result == 0) {
						result = this->balanceRemoval(node);
					}
				} else {
					result = 133;
				}
			}
		}

		return result;
	}

	/**
	 * Makes sure that we replace the node with a nonnull node
	 */	
	virtual int replaceNodeWithTheOnlyChild(typename BinTree<T,S>::BinNode * bnode) {
		int result = 0;
		typename BinTree<T,S>::BinNode * rep = NULL;
		RBNode * node = (RBNode *) bnode;

		if (node->childCount() != 1) {
			result = 3;
		}

		if (result == 0) {
			if (!((RBNode *) node->left())->isNull()) {
				rep = node->left();
				node->setLeft(NULL);
			} else if (!((RBNode *) node->right())->isNull()) {
				rep = node->right();
				node->setRight(NULL);
			} else {
				result = 2;
			}
		}

		if (result == 0) {
			// result = node->replaceWithNode(rep);
			result = this->replaceNodeWithNode(node, rep);
		}

		return result;
	}

	/**
	 *	When a black node is deleted and replaced by a black child, the child is marked as double black
	 */
	virtual int replaceNodeWithNode(typename BinTree<T,S>::BinNode * bnode, typename BinTree<T,S>::BinNode * breplacement) {
		RBNode * node = (RBNode *) bnode;
		RBNode * replacement = (RBNode *) breplacement;

		if (!replacement) {
			replacement = new RBNodeNull;
		}

		int result = this->BinTree<T,S>::replaceNodeWithNode(node, replacement);

		if (result == 0) {
			if ((node->color() == kRBTreeNodeColorBlack) && (replacement->color() == kRBTreeNodeColorBlack)) {
				unsigned char currCount = replacement->colorCount();
				replacement->setColorCount(++currCount);
			} else {
				replacement->setColor(kRBTreeNodeColorBlack);
			}
		}

		return result;
	}

	static const unsigned char ROTATION_CASE_LL = 1; 
	static const unsigned char ROTATION_CASE_LR = 2;
	static const unsigned char ROTATION_CASE_RR = 3;
	static const unsigned char ROTATION_CASE_RL = 4;

	/**
	 * Returns the rotation case
	 * Cases:
	 * 	- 0: unknown
	 * 	- 1: ll
	 * 	- 2: lr
	 * 	- 3: rr
	 * 	- 4: rl
	 */
	unsigned char rotationCase(RBNode * node) {
		if (!node) return 0;
		else if (RBTree<T>::isNodeBlack(node)) return 0;
		else if (node->isRoot()) return 0;
		else if (node->childType() == 0) return 0;
		else {
			switch (this->getNodeParent(node)->childType()) {
				case -1:
					switch (node->childType()) {
						case -1:
							//return 1;
							return ROTATION_CASE_LL;
						case 1:
							//return 2;
							return ROTATION_CASE_LR;
						default:
							return 0;
					}
				case 1:
					switch (node->childType()) {
						case -1:
							//return 4;
							return ROTATION_CASE_RL;
						case 1:
							//return 3;
							return ROTATION_CASE_RR;
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
	 *
	 * param 'rotationCase': Must provide how you want the rotation to occur
	 *
	 * Caller must do recolor before calling this method
	 */
	int rotate(RBNode * node, unsigned char rotationCase) {
		int result = 0;
		// RBNode * parent = (RBNode *) node->_parent;
		RBNode * parent = (RBNode *) this->getNodeParent(node);
		RBNode * grandparent = node->grandParent();
		RBNode * tmp = NULL;
		const unsigned char ll = ROTATION_CASE_LL;
		const unsigned char lr = ROTATION_CASE_LR;
		const unsigned char rr = ROTATION_CASE_RR;
		const unsigned char rl = ROTATION_CASE_RL;
		unsigned char rcase = rotationCase;
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
				tmp->setLeft(new RBNodeNull);
				//tmp->left()->_location = tmp->left()->_location;
				this->setNodeLocation(tmp->left(), tmp->leftAddr());
				// tmp->left()->_parent = tmp;
				this->setNodeParent(tmp->left(), (typename BinTree<T,S>::BinNode *) tmp);
				break;
			case 1:
				tmp->setRight(new RBNodeNull);
				// tmp->right()->_location = tmp->right()->_location;
				this->setNodeLocation(tmp->right(), tmp->rightAddr());
				// tmp->right()->_parent = tmp;
				this->setNodeParent(tmp->right(), (typename BinTree<T,S>::BinNode *) tmp);
				break;
			default:
				result = 111;
				break;
		}
		
		if (result == 0) {
			tmp = NULL;
			if ((rcase == ll) || (rcase ==  rr)) {
				// Put parent in grandparent's location
				// *grandparent->_location = parent;
				*this->getNodeLocation(grandparent) = parent;
				// *parent->_location = new RBNodeNull; // make sure gp has a null node to replace parent
				*this->getNodeLocation(parent) = new RBNodeNull;
				// (*parent->_location)->_parent = grandparent;
				this->setNodeParent(
					*this->getNodeLocation(parent),
					(typename BinTree<T,S>::BinNode *) grandparent
				);
				// (*parent->_location)->_location = parent->_location;
				this->setNodeLocation(
					*this->getNodeLocation(parent),
					this->getNodeLocation(parent)
				);
				// parent->_location = grandparent->_location;
				this->setNodeLocation(parent, this->getNodeLocation((typename BinTree<T,S>::BinNode *) grandparent));
				// parent->_parent = grandparent->_parent; // should work if gp is root
				this->setNodeParent(parent, this->getNodeParent((typename BinTree<T,S>::BinNode *) grandparent));

				// Clear grandparent's position memory
				// grandparent->_parent = NULL;
				// grandparent->_location = NULL;
				this->setNodeParent(grandparent, NULL);
				this->setNodeLocation(grandparent, NULL);

				// Find new spot for grandparent
				if (rcase == ll) {
					tmp = (RBNode *) parent->right();
					newLocation = (RBNode **) parent->rightAddr();
				} else {
					tmp = (RBNode *) parent->left();
					newLocation = (RBNode **) parent->leftAddr();
				}

				// Do the replacement
				// grandparent->_parent = parent;
				this->setNodeParent(grandparent, (typename BinTree<T,S>::BinNode *) parent);
				*newLocation = grandparent;
				// grandparent->_location = (typename BinTree<T,S>::BinNode **) newLocation;
				this->setNodeLocation(grandparent, (typename BinTree<T,S>::BinNode **) newLocation);

				// Figuring out new spot for the node
				// grandparent is replacing
				if (!tmp->isNull()) {
					// tmp->_parent = grandparent;
					this->setNodeParent(tmp, (typename BinTree<T,S>::BinNode *) grandparent);
					if (rcase == ll) {
						newLocation = (RBNode **) grandparent->leftAddr();
					} else {
						newLocation = (RBNode **) grandparent->rightAddr();
					}
				}
			} else if ((rcase == lr) || (rcase == rl)) {
				// Put node in parent's position
				// node->_parent = parent->_parent;
				this->setNodeParent(node, this->getNodeParent(parent));
				// *node->_location = new RBNodeNull; // Make sure parent has a null node we node used to be
				*this->getNodeLocation(node) = new RBNodeNull;
				// (*node->_location)->_parent = parent;
				this->setNodeParent(
					*this->getNodeLocation(node),
					this->getNodeParent(node)
				);
				// (*node->_location)->_location = node->_location;
				this->setNodeLocation(
					*this->getNodeLocation(node),
					this->getNodeLocation(node)
				);
				// node->_location = parent->_location;
				this->setNodeLocation(node, parent->_location);
				// *node->_location = node;
				*this->getNodeLocation(node) = node;
				//parent->_parent = NULL;
				this->setNodeParent((typename BinTree<T,S>::BinNode *) parent, NULL);
				// parent->_location = NULL;
				this->setNodeLocation((typename BinTree<T,S>::BinNode *) parent, NULL);

				// Find where the parent will be next
				if (rcase == lr) {
					tmp = (RBNode *) node->left();
					newLocation = (RBNode **) node->leftAddr();
				} else {
					tmp = (RBNode *) node->right();
					newLocation = (RBNode **) node->rightAddr();
				}

				// Do the replacement
				// parent->_parent = (typename BinTree<T,S>::BinNode *) node;
				this->setNodeParent(parent, (typename BinTree<T,S>::BinNode *) node);
				*newLocation = parent;
				//parent->_location = (typename BinTree<T,S>::BinNode **) newLocation;
				this->setNodeLocation(parent, (typename BinTree<T,S>::BinNode **) newLocation);

				// Find the new spot for the node
				// parent is replacing
				if (!tmp->isNull()) {
					//tmp->_parent = parent;
					this->setNodeParent(tmp, (typename BinTree<T,S>::BinNode *) parent);
					if (rcase == lr) {
						newLocation = (RBNode **) parent->rightAddr();
					} else {
						newLocation = (RBNode **) parent->leftAddr();
					}
				}
			
			// no rotation case
			} else {
				result = 117;
			}
		}

		// If the rotation orphaned a node (tmp), then
		// we need to put it in the newLocation
		if (result == 0) {
			if (!tmp->isNull()) {
				if (!(*newLocation)->isNull()) result = this->insert(tmp, *newLocation);
				else {
					// IF newLocation was holding a null node, we need
					// to delete the null node memory
					Delete(*newLocation);

					*newLocation = tmp;
					//tmp->_location = (typename BinTree<T,S>::BinNode **) newLocation;
					this->setNodeLocation((typename BinTree<T,S>::BinNode *) tmp, (typename BinTree<T,S>::BinNode **) newLocation);
				}
			}
		}

		return result;
	}

	virtual const typename BinTree<T,S>::BinNode * maxNode(const typename BinTree<T,S>::BinNode * node) const {
		const RBNode * rnode = (const RBNode *) node;
		if (!rnode) return NULL;
		else if (rnode->isNull()) return rnode;
		else if (!rnode->right()->isNull()) return this->maxNode(rnode->right());
		else return rnode;
	}
	
	virtual const typename BinTree<T,S>::BinNode * minNode(const typename BinTree<T,S>::BinNode * node) const {
		const RBNode * rnode = (const RBNode *) node;
		if (!rnode) return NULL;
		else if (rnode->isNull()) return rnode;
		else if (!rnode->left()->isNull()) return this->minNode(rnode->left());
		else return rnode;
	}
	
	/**
	 * Ensures we do not print the null nodes
	 */	
	virtual void print(RBNode * rnode, bool printNullNodes) {
		if (rnode->right()) {
			bool isNull = rnode->right()->isNull();
			if (!isNull || (printNullNodes && isNull))
				this->print((RBNode *) rnode->right(), printNullNodes);
		}

		rnode->print();
	
		if (rnode->left()) {
			bool isNull = rnode->left()->isNull();
			if (!isNull || (printNullNodes && isNull))
				this->print((RBNode *) rnode->left(), printNullNodes);
		}
	}

	/**
	 * 	We want to make sure we consider the null nodes
	 */	
	virtual int locateLeafNodes(List<const typename BinTree<T,S>::BinNode *> * outList, const typename BinTree<T,S>::BinNode * node) {
		int result = 0;
		RBNode * rnode = (RBNode *) node;

		if (!rnode) return 114;
		else if (rnode->childCount() > 0) {
			if (!rnode->left()->isNull()) result = this->locateLeafNodes(outList, node->left());

			if (result == 0)
				if (!rnode->right()->isNull()) result = this->locateLeafNodes(outList, node->right());

			return result;
		} else {
			result = outList->add(node);
			return result;
		}
	}
};

} // namespace BF

#endif // RBTREE_HPP

