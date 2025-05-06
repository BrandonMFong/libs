/**
 * author: Brando
 * date: 7/31/22
 */

#ifndef LIST_HPP
#define LIST_HPP

#include "access.hpp"
#include "vector.hpp"
#include "exception.hpp"
#include "swap.hpp"
#include <iostream>
#include <initializer_list>

namespace BF {

template<typename T> struct Sort;

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
 *
 * Memory:
 * Unless a callback is specified, by default the node
 * object memory will not be deallocated.
 */
template <typename L, typename S = long>
class List : public Vector<L,S> {
public:
	friend struct Sort<List>;
	
	/**
	 * Bi-directional node
	 */
	class Node : public Object {
		friend List<L,S>;
	
		friend struct Sort<List>;
	public:
		Node * next() const {
			return this->right;
		}

		Node * prev() const {
			return this->left;
		}

		L object() const {
			return this->obj;
		}

	private:
		Node() : Object() {
			this->left = 0;
			this->right = 0;
			this->obj = 0;
			this->list = NULL;
		}

		virtual ~Node() {
			if (this->list && this->list->_nodeObjectCleanUpCallback) {
				this->list->_nodeObjectCleanUpCallback(this->obj);
			}

			this->left = 0;
			this->right = 0;
		}

		L & refobject() {
			return this->obj;
		}


		Node * left;
		Node * right;
		L obj;

		const List * list;
	};

public:

	List() : Vector<L,S>() {
		this->_head = 0;
		this->_tail = 0;
		this->_count = 0;
		this->_nodeObjectCleanUpCallback = 0;
		this->_compareCallback = 0;
	}

	List(const std::initializer_list<L> & list) : List() {
		this->set(list);
	}

	virtual ~List() {
		this->deleteAll();
	}

	S count() const { return this->_count; }
	virtual S size() const { return this->count(); }

	/**
	 * returns max object in list
	 *
	 * throws if count is 0, caller must check
	 */
	virtual L max() const {
		if (this->count() == 0) {
			throw Exception("cannot get max from empty list");
		}
		Node * maxnode = this->first();
		for (Node * n = maxnode->next();
			n; n = n->next()) {
			if (maxnode->object() < n->object()) {
				maxnode = n;
			}
		}

		return maxnode->object();
	}

	// Adds obj at tail end of list
	int add(L obj) {
		int result = 0;

		Node * n = new Node;
		n->obj = obj;
		n->list = this;

		// If we are first adding something
		if (!this->_head && !this->_tail) {
			this->_head = n;
			this->_tail = n;

		// If we already have a list of things
		} else if (this->_head && this->_tail) {
			n->left = this->_tail;
			this->_tail->right = n;
			this->_tail = n;

		// this case shouldn't happen
		} else {
			result = 1;
		}

		if (!result)
			this->_count++;

		return result;
	}

	/**
	 * finds object and removes from list
	 *
	 * DOES NOT DELETE MEMORY
	 */
	int pluckObject(L obj) {
		for (Node * n = this->first(); n; n = n->next()) {
			if (!this->runCompare(obj, n->object())) {
				return this->deleteNode(n);
			}
		}

		return 2;
	}

	/**
	 * deletes object at the index
	 *
	 * index: must not exceed bounds of array
	 */
	int deleteObjectAtIndex(S index) {
		if ((index >= 0) && (index < this->_count))
			return this->deleteObjectAtIndex(index, this->_head, 0);
		else return 1;
	}

	/**
	 * Insert obj at index.  Will push every object after an index 
	 *
	 * index: must be within range of list.
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
		Node * node = this->_head;
		while (node) {
			Node * tmp = node->right;
			delete node;
			node = tmp;
			this->_count--;
		}

		this->_head = NULL;
		this->_tail = NULL;
	}

	// returns object at index
	// returns 0 if an error ocurred
	virtual L objectAtIndex(S index) const {
		Node * n = this->nodeAtIndex(index, this->_head);
		if (n) return n->object();
		else return 0;
	}

	virtual L & refObjectAtIndex(S index) {
		Node * n = this->nodeAtIndex(index, this->_head);
		if (n) return n->refobject();
		throw Exception("no object found at %d", (int) index);
	}

	/**
	 * sets release callback
	 */
	void setReleaseCallback(void (* callback) (L obj)) {
		this->_nodeObjectCleanUpCallback = callback;
	}

	/**
	 * Sets the callback we use when sorting
	 */
	void setCompareCallback(int (* callback) (L a, L b)) {
		this->_compareCallback = callback;
	}

	/**
	 * Returns the head of the list
	 *
	 * Caller can use this node to traverse through the 
	 * list
	 */
	Node * first() const {
		return this->_head;
	}

	Node * last() const {
		return this->_tail;
	}

	/**
	 * Checks if there is at least one node with obj
	 */
	bool contains(const L obj) {
		return this->getNodeForObject(obj, this->_head) != NULL;
	}

	/**
	 * Prints entire list
	 */
	void print() {
		std::cout << std::endl;
		std::cout << "[";

		this->print(this->_head, NULL);

		std::cout << "]";
		std::cout << std::endl;
	}

	/**
	 * Shuffles node links
	 */
	int shuffle() {
		srand(time(0));
		Node * n = this->_tail; // get last
		int i = this->count() - 1; // get last's index
		for (; n; n = n->prev()) {
			// Get random node
			int r = rand() % (i + 1);
			Node * tmp = this->nodeAtIndex(r, this->_head);
			BF::swap<L>(n->obj, tmp->obj); // swap nodes
			i--;
		}

		return 0;
	}

protected:

	/**
	 * Delete node but does not delete object
	 */
	int deleteNode(Node * node) {
		// If currNode is the tail, reset the tail var 
		// to the currNode's left
		if (this->_tail == node) this->_tail = node->left;
		if (this->_head == node) this->_head = node->right;

		// Close connection
		if (node->left)
			node->left->right = node->right;

		if (node->right)
			node->right->left = node->left;
	
		// This method does not delete the object memory
		node->list = NULL;

		// Delete this node at index
		delete node;

		this->_count--;

		return 0;
	}

private:

	/**
	 * Allows us to set list with {...} notation
	 *
	 * Calling this will reset the list to the input
	 */
	void set(const std::initializer_list<L> & list) {
		this->deleteAll();
		typename std::initializer_list<L>::iterator itr;
		for (itr = list.begin(); itr != list.end(); ++itr) {
			this->add(*itr);
		}
	}

	/**
	 * Allows us to set our data from a fixed sequenced collection
	 */	
	void set(L * rawArray, S size) {
		if (!rawArray) return; // don't do anything
		this->deleteAll();
		for (S i = 0; i < size; i++) {
			this->add(rawArray[i]);
		}
	}

	/**
	 * iteratively traverses through linked list until we read the reqIndex'th node
	 *
	 * we start with the search from the `node` param
	 * 
	 * if reqIndex < 0, returns NULL
	 */
	Node * nodeAtIndex(S reqIndex, Node * node) const {
		if (reqIndex < 0) return NULL;

		while (--reqIndex >= 0 && node) {
			node = node->right;
		}

		return node;
	}

	/**
	 * If position is found, this will effectively push currNode (and 
	 * following nodes) up one index and set obj's node to take its place
	 *
	 * We do not need to worry about head or tail since we should not be
	 * inserting at the end of the list (if so then caller should use 
	 * `add`)
	 */
	int insertObjectAtIndex(L obj, S reqIndex, Node * currNode, S currIndex) {
		if (currNode == 0) {
			return 2; // We assume we are inserting within the range of the current list
		} else {
			if (currIndex < reqIndex) {
				return this->insertObjectAtIndex(obj, reqIndex, currNode->right, ++currIndex);
			} else {
				Node * n = new Node;
				n->obj = obj;
				n->list = this;

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
	 *
	 * Deletes currNode and pulls everything forward
	 */
	int deleteObjectAtIndex(S reqIndex, Node * currNode, S currIndex) {
		if (currNode == 0) return -1;
		else {
			if (currIndex < reqIndex) {
				return this->deleteObjectAtIndex(reqIndex, currNode->right, ++currIndex);
			} else {
				L object = currNode->object();
				if (this->_nodeObjectCleanUpCallback) this->_nodeObjectCleanUpCallback(object);
				return this->deleteNode(currNode);
			}
		}
	}

	/**
	 * Returns the first node for obj
	 * 
	 * Returns null if nothing was found
	 */
	Node * getNodeForObject(L obj, Node * node) {
		if (!node) return NULL;
		else {
			if (node->obj == obj) return node; // First matching object
			else {
				return this->getNodeForObject(obj, node->right); // Keep traversing
			}
		}
	}

	void print(Node * node, Node * end) {
		if (node) {
			std::cout << " " <<  node->object() << " ";

			if (node != end)
				this->print(node->right, end);
		}
	}

	/**
	 * a < b : < 0
	 * a > b : > 0
	 * a == b : 0
	 */
	int runCompare(L a, L b) {
		if (this->_compareCallback) return this->_compareCallback(a, b);
		else {
			if (a < b) return -1;
			else if (a > b) return 1;
			else return 0;
		}
	}

	// Holds the first node
	Node * _head;

	// Holds the last node in list
	Node * _tail;

	// Keeps count of how many nodes are linked
	S _count;

	/**
	 * Holds pointer to a pointer outlining how the 
	 * Node::obj's memory should be handled at deletion
	 */
	void (* _nodeObjectCleanUpCallback)(L obj);

	/**
	 * Used to compare node objects
	 * a < b : < 0
	 * a > b : > 0
	 * a == b : 0
	 */
	int (* _compareCallback) (L a, L b);
	
public:

	void operator=(const std::initializer_list<L> & list) {
		this->set(list);
	}

	virtual L operator[](S index) const {
		return this->objectAtIndex(index);
	}

	virtual L & operator[](S index) {
		return this->refObjectAtIndex(index);
	}

public:

	/**
	 * Range-Based implementation for our Linked list
	 * ref: https://en.cppreference.com/w/cpp/language/range-for
	 *
	 * this can probably go to the collections class
	 */
	class Iterator {
	private:
		Node * _curr;
	public:
		Iterator(Node * n) : _curr(n) {}
		L operator*() const { 
			return this->_curr->object();
		}
		Iterator & operator++() { // pre-inc
			this->_curr = this->_curr->next();
			return *this;
		}
		Iterator operator++(int) { // post-inc
			Iterator old = *this;
			this->operator++();
			return old;
		}
		bool operator!=(const Iterator& i) {
			return !(*this == i);
		}
		bool operator==(const Iterator& i) {
			return this->_curr == i._curr;
		}
	};

	/// required interfaces: begin() & end()
	Iterator begin() const { return this->first(); }
	Iterator end() const { return NULL; }
};

} // namespace BF

#endif // LIST_HPP

