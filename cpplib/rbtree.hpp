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
			this->_colorSpace = 0;

			this->setColor(kRBTreeNodeColorRed);
			this->setColorCount(1);
		}

		virtual ~RBNode() {}
		virtual bool isNull() { return false; }

		virtual void setLeft(BinTree<T,S>::BinNode * left) { this->BinTree<T,S>::BinNode::setLeft(left); }
		virtual RBNode * left() { return (RBNode *) this->BinTree<T,S>::BinNode::left(); }

		virtual void setRight(BinTree<T,S>::BinNode * right) { this->BinTree<T,S>::BinNode::setRight(right); }
		virtual RBNode * right() { return (RBNode *) this->BinTree<T,S>::BinNode::right(); }

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
			
			if (this->color() == kRBTreeNodeColorRed) std::cout << "\033[0;31m";

			for (int i = 0; i < this->colorCount(); i++) std::cout << "[";
			this->printObject();
			for (int i = 0; i < this->colorCount(); i++) std::cout << "]";

			if (this->color() == kRBTreeNodeColorRed) std::cout << "\033[0m";
			std::cout << std::endl;
		}

		virtual void printObject() { std::cout << this->obj; }

		/**
	 	 *	When a black node is deleted and replaced by a black child, the child is marked as double black
		 */
		virtual int replaceWithNode(BinTree<T,S>::BinNode * replacement) {
			if (!replacement) {
				replacement = new RBNodeNull;
			}

			int result = this->BinTree<T,S>::BinNode::replaceWithNode(replacement);

			if (result == 0) {
				if ((this->color() == kRBTreeNodeColorBlack) && (((RBNode *) replacement)->color() == kRBTreeNodeColorBlack)) {
					unsigned char currCount = ((RBNode *) replacement)->colorCount();
					((RBNode *) replacement)->setColorCount(++currCount);
				} else {
					((RBNode *) replacement)->setColor(kRBTreeNodeColorBlack);
				}
			}

			return result;
		}
	
		virtual unsigned char childCount() {
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

		virtual RBNode * grandParent() override {
			return (RBNode *) this->BinTree<T,S>::BinNode::grandParent();
		}
	
		virtual RBNode * pibling() override {
			return (RBNode *) this->BinTree<T,S>::BinNode::pibling();
		}
	
		virtual RBNode * sibling() override {
			return (RBNode *) this->BinTree<T,S>::BinNode::sibling();
		}

		char color() {
			return this->_colorSpace & kRBTreeNodeMaskColor;
		}

		void setColor(char color) {
			this->_colorSpace = (this->_colorSpace & ~kRBTreeNodeMaskColor) | (color & kRBTreeNodeMaskColor);
		}

		unsigned char colorCount() {
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
		
		virtual bool isNull() { return true; }

		virtual void printObject() { std::cout << "_"; }
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
		virtual bool isNull() { return false; }
	
		virtual void setLeft(BinTree<T,S>::BinNode * left) { 
			if (left) this->RBNode::setLeft(left);
			else {
				this->RBNode::setLeft(new RBNodeNull);
				this->left()->location = this->leftAddr();
				this->left()->parent = this;
			}
		}

		virtual void setRight(BinTree<T,S>::BinNode * right) {
			if (right) this->RBNode::setRight(right);
			else {
				this->RBNode::setRight(new RBNodeNull);
				this->right()->location = this->rightAddr();
				this->right()->parent = this;
			}
		}
	};

PUBLIC:

	RBTree() : BinTree<T,S>() {}

	virtual ~RBTree() {}
	
	RBNode * root() override { return (RBNode *) this->BinTree<T,S>::root(); }

	/**
	 * inserts object into tree
	 */
	virtual int insert(T obj) {
		RBNode * newNode = (RBNode *) this->createNode();
		newNode->obj = obj;

		if (this->root()) {
			return this->insert(newNode, this->root());
		} else {
			newNode->setColor(kRBTreeNodeColorBlack);
			this->setRoot(newNode);
			this->root()->location = this->rootAddr();
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
		bool isRoot = node->isRoot();

		if (!node) return 101;
		else {
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
	}

	virtual void print() {
		this->print(false);
	}

	virtual void print(bool printNullNodes) {
		std::cout << std::endl;
		this->print(this->root(), printNullNodes);
		std::cout << std::endl;
	}

PRIVATE:

	virtual void * createNode() {
		return new RBNodeNonnull;
	}
	
	static bool isNodeBlack(RBNode * node) {
		if (!node) return true;
		else if (node->isNull()) return true;
		else return node->color() == kRBTreeNodeColorBlack;
	}

	static bool isNodeRed(RBNode * node) {
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
		} else if (RBTree<T>::isNodeBlack((RBNode *) node->parent)) {
			return 0;
		} else if (RBTree<T>::isNodeRed((RBNode *) node->parent)) {
			// Involves rotations
			if (RBTree<T>::isNodeBlack(node->pibling())) {
				RBNode * parent = (RBNode *) node->parent;
				unsigned char rcase = RBTree<T>::rotationCase(node);

				if ((rcase == ROTATION_CASE_RR) || (rcase == ROTATION_CASE_LL)) {
					if ((tmp = (RBNode *) node->parent)) {
						tmp->setColor(kRBTreeNodeColorBlack);
					}

					if ((tmp = node->grandParent())) {
						tmp->setColor(kRBTreeNodeColorRed);
					}
				}

				int result = this->rotate(node, rcase);
				
				if (result == 0) {
					if ((rcase == ROTATION_CASE_RL) || (rcase == ROTATION_CASE_LR)) {
						if ((tmp = (RBNode *) parent->parent)) {
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
				if (node->parent) {
					((RBNode *) node->parent)->setColor(kRBTreeNodeColorBlack);
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
		RBNode * parent = (RBNode *) node->parent;
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
	virtual int replaceNodeWithTheOnlyChild(typename BinTree<T,S>::BinNode * node) {
		int result = 0;
		typename BinTree<T,S>::BinNode * rep = NULL;

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
			result = node->replaceWithNode(rep);
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
		RBNode * parent = (RBNode *) node->parent;
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
				tmp->left()->location = tmp->left()->location;
				tmp->left()->parent = tmp;
				break;
			case 1:
				tmp->setRight(new RBNodeNull);
				tmp->right()->location = tmp->right()->location;
				tmp->right()->parent = tmp;
				break;
			default:
				result = 111;
				break;
		}
		
		if (result == 0) {
			tmp = NULL;
			if ((rcase == ll) || (rcase ==  rr)) {
				// Put parent in grandparent's location
				*grandparent->location = parent;
				*parent->location = new RBNodeNull; // make sure gp has a null node to replace parent
				(*parent->location)->parent = grandparent;
				(*parent->location)->location = parent->location;
				parent->location = grandparent->location;
				parent->parent = grandparent->parent; // should work if gp is root

				// Clear grandparent's position memory
				grandparent->parent = NULL;
				grandparent->location = NULL;

				// Find new spot for grandparent
				if (rcase == ll) {
					tmp = (RBNode *) parent->right();
					newLocation = (RBNode **) parent->rightAddr();
				} else {
					tmp = (RBNode *) parent->left();
					newLocation = (RBNode **) parent->leftAddr();
				}

				// Do the replacement
				grandparent->parent = parent;
				*newLocation = grandparent;
				grandparent->location = (typename BinTree<T,S>::BinNode **) newLocation;

				// Figuring out new spot for the node
				// grandparent is replacing
				if (!tmp->isNull()) {
					tmp->parent = grandparent;
					if (rcase == ll) {
						newLocation = (RBNode **) grandparent->leftAddr();
					} else {
						newLocation = (RBNode **) grandparent->rightAddr();
					}
				}
			} else if ((rcase == lr) || (rcase == rl)) {
				// Put node in parent's position
				node->parent = parent->parent;
				*node->location = new RBNodeNull; // Make sure parent has a null node we node used to be
				(*node->location)->parent = parent;
				(*node->location)->location = node->location;
				node->location = parent->location;
				*node->location = node;
				parent->parent = NULL;
				parent->location = NULL;

				// Find where the parent will be next
				if (rcase == lr) {
					tmp = (RBNode *) node->left();
					newLocation = (RBNode **) node->leftAddr();
				} else {
					tmp = (RBNode *) node->right();
					newLocation = (RBNode **) node->rightAddr();
				}

				// Do the replacement
				parent->parent = (typename BinTree<T,S>::BinNode *) node;
				*newLocation = parent;
				parent->location = (typename BinTree<T,S>::BinNode **) newLocation;

				// Find the new spot for the node
				// parent is replacing
				if (!tmp->isNull()) {
					tmp->parent = parent;
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
					tmp->location = (typename BinTree<T,S>::BinNode **) newLocation;
				}
			}
		}

		return result;
	}

	virtual typename BinTree<T,S>::BinNode * maxNode(typename BinTree<T,S>::BinNode * node) {
		RBNode * rnode = (RBNode *) node;
		if (!rnode) return NULL;
		else if (rnode->isNull()) return rnode;
		else if (!rnode->right()->isNull()) return this->maxNode(rnode->right());
		else return rnode;
	}
	
	virtual typename BinTree<T,S>::BinNode * minNode(typename BinTree<T,S>::BinNode * node) {
		RBNode * rnode = (RBNode *) node;
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
	virtual int locateLeafNodes(List<typename BinTree<T,S>::BinNode *> * outList, typename BinTree<T,S>::BinNode * node) {
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

#endif // RBTREE_HPP

