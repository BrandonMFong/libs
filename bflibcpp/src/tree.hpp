/**
 * author: brando
 * date: 11/28/24
 */

#ifndef TREE_HPP
#define TREE_HPP

#include "collection.hpp"
#include "release.hpp"
#include "exception.hpp"
#include "compare.hpp"
#include "allocator.hpp"

extern "C" {
#include <bflibc/tree.h>
}

namespace BF {

/**
 * AVL Tree
 */
template<typename T, typename S = size_t, class C = Compare<T>, class A = Allocator<T>>
class Tree : public Collection<S> {
	/**
	 * holds container of type T
	 */
	class Container : public Object {
	public:
		T _obj;
		const Tree * _treeRef;
		void (*_release)(T obj);
		Container(T obj, const Tree * treeRef, void (*release)(T obj))
		: _obj(obj), _treeRef(treeRef), _release(release), Object() {
			BFRetain(this->_treeRef);
		}
		virtual ~Container() {
			if (this->_release) {
				this->_release(this->_obj);
			}
			BFRelease(this->_treeRef);
		}
	};

public:

	/**
	 * wraps BFTreeNode
	 */
	class Node : public Object {
		BFTreeNode _node;
	public:
		Node(const Node & node) : Node(node._node) { }
		Node(BFTreeNode node) : _node(node), Object() { }
		virtual ~Node() { }
		Node left() const { return BFTreeNodeGetLeft(this->_node); }
		Node right() const { return BFTreeNodeGetRight(this->_node); }
		T & object() const {
			Container * cont = (Container *) BFTreeNodeGetObject(this->_node);
			if (!cont) {
				throw Exception("object is null");
			}
			return cont->_obj;
		}

		/// null if value is a null node
		bool isNull() const { return this->_node == NULL; }
	};

	Tree() : Collection<S>() {
		this->_tree = BFTreeCreate();
		if (!this->_tree) return;
		BFTreeSetCompare(this->_tree, this->_compare);
		BFTreeSetRelease(this->_tree, this->_BFTreeRelease);
	}

	Tree(bool allowDuplicates) : Tree() {
		BFTreeSetAllowDuplicates(this->_tree, allowDuplicates);
	}

	virtual ~Tree() {
		BFTreeRelease(this->_tree);
	}

	/**
	 * defines how the objects are organized in the
	 * tree
	 *
	 * see _compare
	 */
	[[deprecated("Please use BF::Compare functor")]]
	void setCompare(int (*compare)(const T & a, const T & b)) { }

	/**
	 * defines how objects are released
	 */
	void setRelease(void (*release)(T obj)) {
		this->_release = release;
	}

	/**
	 * true if tree can allow duplicates
	 */
	bool allowDuplicates() const {
		return BFTreeGetAllowDuplicates(this->_tree);
	}

	/**
	 * returns amount of tree nodes
	 */
	S size() const {
		return BFTreeSize(this->_tree);
	}

	/**
	 * inserts new object into tree
	 */
	int insert(T object) {
		if (!this->_tree) return -1;
		Container * c = new Container(object, this, this->_release);
		return BFTreeInsert(this->_tree, c);
	}

	/**
	 * removes object from tree
	 */
	int remove(T object) {
		if (!this->_tree) return -1;
		Container c(object, this, NULL);
		return BFTreeRemove(this->_tree, &c);
	}

	/**
	 * true if tree contains object
	 */
	bool contains(T object) const {
		if (!this->_tree) return -1;
		Container c(object, this, NULL);
		return BFTreeContains(this->_tree, &c);
	}

	/**
	 * returns root node
	 */
	Node root() const {
		return BFTreeGetRoot(this->_tree);
	}

private:

	/**
	 * compare: how each object is compared to each other
	 *	a < b -> result < 0 
	 *	a > b -> result > 0
	 *	a == b -> result == 0
	 */
	static int _compare(BFTreeObject a, BFTreeObject b) {
		if (!a || !b) return 0;
		Container * acont = (Container *) a;
		Container * bcont = (Container *) b;

		C cmp;
		return cmp(acont->_obj, bcont->_obj);
	}

	void (*_release)(T obj);
	static void _BFTreeRelease(BFTreeObject object) {
		Container * cont = (Container *) object;
		BFRelease(cont);
	}

	BFTree _tree;
};
}

#endif // TREE_HPP

