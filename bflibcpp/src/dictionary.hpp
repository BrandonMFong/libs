/**
 * author: Brando
 * date: 10/4/22
 */

#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include "rbtree.hpp"
#include "access.hpp"
#include "object.hpp"

namespace BF {

/**
 * Dictionary whose entries are pair with keys (of type K) and values (of type V)
 */
template <typename K, typename V, typename S = int>
class Dictionary : public Object {
PUBLIC:

	/**
	 * Holds an entry for value that can be located by key
	 */
	class Entry : public Object {
		friend class Dictionary<K,V,S>;
	PUBLIC:
		/**
		 * Used for tree to organize
		 *
		 * a == b -> 0
		 * a < b -> -1
		 * a > b -> 1
		 */
		static int Compare(Entry * a, Entry * b) {
			return a->runCompare(b);
		}

		K key() const { return this->_key; }
		V value() const { return this->_value; }

	PRIVATE:

		Entry(K k, V v, Dictionary<K,V,S> * dictRef) : Object() {
			this->_dictRef = dictRef;

			// See how the dict owner wants to retain key and values
			if (this->_dictRef->_keyValueRetain) {
				this->_dictRef->_keyValueRetain(&k, &v);
			}

			this->_key = k;
			this->_value = v;
		}

		virtual ~Entry() {
			if (this->_dictRef->_keyValueRelease) {
				this->_dictRef->_keyValueRelease(&this->_key, &this->_value);
			}
		}

		/**
		 * Compares our key with input entry's
		 *
		 * If no comparison callback was defined, then we will simply just
		 * compare raw values of _key
		 */
		int runCompare(Entry * b) const {
			return this->_dictRef->runCompare(this->key(), b->key());
		}

		K _key;
		V _value;

		/**
		 * A nonnull reference to our dictionary
		 */
		const Dictionary<K,V,S> * _dictRef;
	};
	
	friend class Entry;

	Dictionary() : Object() {
		this->_keyCompare = 0;
		this->_keyValueRelease = 0;
		this->_keyValueRetain = 0;

		// Establish how the tree will compare the entries
		this->_tree.setCompareCallback(Entry::Compare);
	}

	Dictionary(
		int (* keyCompareCallback) (K a1, K a2), 
		void (* retainCallback) (K * k, V * v), 
		void (* releaseCallback) (K * k, V * v)) : Dictionary() {
		this->_keyCompare = keyCompareCallback;
		this->_keyValueRelease = releaseCallback;
		this->_keyValueRetain = retainCallback;
	}

	virtual ~Dictionary() {
		this->_keyCompare = 0;
		this->_keyValueRelease = 0;
		this->_keyValueRetain = 0;
	}

	/**
	 * Returns number of key/value pairs 
	 */
	S size() const {
		return this->_tree.count();
	}

	/**
	 * Defines how the entries will be organized in dictionary.
	 *
	 * a1 == a2 -> 0
	 * a1 < a2 -> -1
	 * a1 > a2 -> 1
	 */
	void setKeyCompareCallback(int (* cb) (K a1, K a2)) {
		this->_keyCompare = cb;
	}

	/** 
	 * Defines how the keys and values memory gets deleted
	 */
	void setEntryReleaseCallback(void (* cb) (K * k, V * v)) {
		this->_keyValueRelease = cb;
	}

	/**
	 * Defines how the entry will retain the memory for
	 * keys and values
	 */
	void setEntryRetainCallback(void (* cb) (K * k, V * v)) {
		this->_keyValueRetain = cb;
	}

	/**
	 * Returns value for key
	 */
	V valueForKey(K key) {
		int error = 0;
		Entry * obj = this->getEntryForKey(key, this->_tree.root(), &error);

		if (!error && obj) {
			return obj->value();
		}

		return 0;
	}

	/**
	 * Pairs key to value and creates an entry in the dictionary
	 */
	int setValueForKey(K key, V value) {
		Entry * ent = new Entry(key, value, this);
		return this->_tree.insert(ent);
	}

	/**
	 * Removes key value pair
	 */
	int removeValueForKey(K key) {
		int error = 0;
		const typename RBTree<Entry *>::RBNode * n = this->getEntryNodeForKey(key, this->_tree.root(), &error);

		if (n && !error) {
			return this->_tree.deleteNode(n);
		} else {
			return error == 0 ? 15 : error;
		}
	}

	/**
	 * Prints inorder traversal of tree
	 */
	void print() {
		this->print(this->_tree.root());
	}

	typedef typename RBTree<Entry *, S>::Iterator Iterator;

	int createIterator(Iterator ** itr) {
		return this->_tree.createIterator(itr);
	}
PRIVATE:

	/**
	 * inorder traversal
	 */
	void print(const typename RBTree<Entry *>::RBNode * node) {
		if (node) {
			if (!node->isNull()) {
				this->print(node->left());

				Entry * entry = node->object();
				if (entry) {
					std::cout << entry->key() << " : " << entry->value() << std::endl;
				}

				this->print(node->right());
			}
		}
	}

	/**
	 * Returns the first entry object that has the key
	 *
	 * There may be possibility that there are two entries with the same
	 * key. Should I not allow that?
	 */
	Entry * getEntryForKey(K key, const typename RBTree<Entry *>::RBNode * node, int * err) {
		int error = 0;
		const typename RBTree<Entry *>::RBNode * n = this->getEntryNodeForKey(key, node, &error);

		if (n && !error) {
			return n->object();
		} else {
			if (err) *err = error;
			return NULL;
		}
	}
	
	/**
	 * Returns the first entry node that has the key
	 */
	const typename RBTree<Entry *>::RBNode * getEntryNodeForKey(K key, const typename RBTree<Entry *>::RBNode * node, int * err) {
		// If the pointer is null then we will throw an error
		if (node == NULL) {
			if (err) *err = 13;
			return NULL;
		// if we are at a null node, return null
		} else if (node->isNull()) return NULL;

		Entry * obj = NULL;

		// If the object is null then we are going to return null with error
		if ((obj = node->object()) == NULL) {
			if (err) *err = 14;
			return NULL;
		}

		int comp = this->runCompare(key, obj->key());
		if (comp == -1) return this->getEntryNodeForKey(key, node->left(), err);
		else if (comp == 1) return this->getEntryNodeForKey(key, node->right(), err);
		else return node;
	}

	int runCompare(K a1, K a2) const {
		if (this->_keyCompare) return this->_keyCompare(a1, a2);
		else {
			if (a1 < a2) return -1;
			else if (a1 > a2) return 1;
			else return 0;
		}
	}

	/**
	 * Holds all our entries
	 */
	RBTree<Entry *, S> _tree;

	/**
	 * Passed to Entry objects so that they know how to compare they keys
	 *
	 * a1 == a2 -> 0
	 * a1 < a2 -> -1
	 * a1 > a2 -> 1
	 */
	int (* _keyCompare) (K a1, K a2);

	/**
	 * This is how we will release the memory stored in key and value
	 *
	 * Mainly only used by Entry
	 */
	void (* _keyValueRelease) (K * key, V * value);

	/**
	 * Defines how we will retian the key value memory
	 *
	 * Mainly only used by Entry
	 */
	void (* _keyValueRetain) (K * key, V * value);
};

} // namespace BF

#endif // DICTIONARY_HPP

