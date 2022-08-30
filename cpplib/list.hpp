/**
 * author: Brando
 * date: 7/31/22
 */

#ifndef LIST_HPP
#define LIST_HPP

/**
 * Linked List implementation
 *
 * Essentially a mutable array
 *
 * L = object/data type this list will hold onto
 * S = This object has a counter of type S.  Default 
 * is a 32 bit integer.  If you are planning to have 
 * a list with size exceeding the 2^32, you should 
 * change the S value to 'long' or 'long long'
 */
template <typename L, typename S = int>
class List {
private:
	
	/**
	 * Bi-directional node
	 */
	template <typename N>
	class Node {
	public:
		Node() {
			this->left = 0;
			this->right = 0;
			this->obj = 0;
			this->objectDeletionCallback = 0;
		}

		virtual ~Node() {
			if (this->objectDeletionCallback) {
				this->objectDeletionCallback(this->obj);
			}
		}

		Node<N> * left;
		Node<N> * right;
		N obj;
		void (* objectDeletionCallback)(N obj);
	};

public:

	List() {
		this->_head = 0;
		this->_count = 0;
		this->_nodeObjectCleanUpCallback = 0;
	}

	virtual ~List() {
		this->deleteAll();
	}

	S count() { return this->_count; }

	// Adds obj at tail end of list
	int add(L obj) {
		return this->add(&this->_head, 0, obj);
	}

	/**
	 * Insert obj at index.  Will push every object after an index 
	 *
	 * index: must be within range of list
	 */
	int insertObjectAtIndex(L obj, S index) {
		if ((index >= 0) && (index < this->_count)) {
			return this->insertObjectAtIndex(obj, index, this->_head, 0);
		} else {
			return 1;
		}
	}

	// Deletes every object in list
	void deleteAll() {
		this->deleteAll(this->_head);
	}

	// returns object at index
	// returns 0 if an error ocurred
	L objectAtIndex(S index) {
		return this->objectAtIndex(index, this->_head, 0);
	}

	/**
	 * deletes object at the index
	 */
	int deleteObjectAtIndex(S index) {
		if (this->_count > 0) return this->deleteObjectAtIndex(index, this->_head, 0);
		else return 1;
	}

	/**
	 * callback will be a pointer to a function that handles how we will delete object memory
	 */
	void setObjectMemoryHandler(void (* callback)(L obj)) {
		this->_nodeObjectCleanUpCallback = callback;
	}

private:

	/**
	 * Recursively finds a null spot of obj to be added to
	 */
	int add(Node<L> ** currNode, Node<L> * prevNode, L obj) {
		if (currNode == 0) {
			return 1;
		} else if (*currNode != 0) {
			return this->add(&(*currNode)->right, *currNode, obj);
		} else {
			Node<L> * n = new Node<L>;
			n->obj = obj;
			n->objectDeletionCallback = this->_nodeObjectCleanUpCallback;

			// make connection if there was a previous nodea
			if (prevNode) {
				n->left = prevNode;
				prevNode->right = n;
			}

			this->_count++;

			// Save to currNode
			*currNode = n;

			return 0;
		}
	}

	/**
	 * Recursively traverses through linked list until we read the reqIndex'th node
	 */
	L objectAtIndex(S reqIndex, Node<L> * node, S currIndex) {
		if (node) {
			if (currIndex == reqIndex) {
				return node->obj;
			} else {
				return this->objectAtIndex(reqIndex, node->right, ++currIndex);
			}
		} else return 0;
	}

	/**
	 * If position is found, this will effectively push currNode (and 
	 * following nodes) up one index and set obj's node to take its place
	 */
	int insertObjectAtIndex(L obj, S reqIndex, Node<L> * currNode, S currIndex) {
		if (currNode == 0) {
			return 2; // We assume we are inserting within the range of the current list
		} else {
			if (currIndex < reqIndex) {
				return this->insertObjectAtIndex(obj, reqIndex, currNode->right, ++currIndex);
			} else {
				Node<L> * n = new Node<L>;
				n->obj = obj;
				n->objectDeletionCallback = this->_nodeObjectCleanUpCallback;

				// Make new connection
				if (currNode->left) currNode->left->right = n;
				n->left = currNode->left;
				n->right = currNode;
				currNode->left = n;

				this->_count++;

				return 0;
			}
		}
	}

	/**
	 * Finds node at reqIndex for deletion
	 */
	int deleteObjectAtIndex(S reqIndex, Node<L> * currNode, S currIndex) {
		if (currNode == 0) return 2;
		else {
			if (currIndex < reqIndex) {
				return this->deleteObjectAtIndex(reqIndex, currNode->right, ++currIndex);
			} else {
				// Close connection
				currNode->left->right = currNode->right;
				currNode->right->left = currNode->left;

				// Delete this node at index
				delete currNode;

				this->_count--;

				return 0;
			}
		}
	}

	/**
	 * Traverses linked list from the start node and deletes from the 
	 * last node
	 */
	void deleteAll(Node<L> * node) {
		if (node == 0) return;
		if (node->right) this->deleteAll(node->right);
		delete node;
		this->_count--;
	}

	// Holds the first node
	Node<L> * _head;

	// Keeps count of how many nodes are linked
	S _count;

	/**
	 * Holds pointer to a pointer outlining how the 
	 * Node::obj's memory should be handled at deletion
	 */
	void (* _nodeObjectCleanUpCallback)(L obj);
};

#endif // LIST_HPP

