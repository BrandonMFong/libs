/**
 * author: brando
 * date: 11/28/24
 */

#ifndef TREE_HPP
#define TREE_HPP

#include "collection.hpp"
#include "release.hpp"
#include "exception.hpp"

extern "C" {
#include <bflibc/tree.h>
}

namespace BF {
template<typename T, typename S = size_t>
class Tree : public Collection<S> {
	/**
	 * holds container of type T
	 */
	class Container : public Object {
	public:
		T _obj;
		Tree * _treeRef;
		Container(T obj, Tree * treeRef)
		: _obj(obj), _treeRef(treeRef), Object() {
			BFRetain(this->_treeRef);
		}
		virtual ~Container() {
			if (this->_treeRef->_release) {
				this->_treeRef->_release(this->_obj);
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
		const Node left() const { return BFTreeNodeGetLeft(this->_node); }
		const Node right() const { return BFTreeNodeGetRight(this->_node); }
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
		BFTreeSetCompare(this->_tree, this->_BFTreeCompare);
		BFTreeSetRelease(this->_tree, this->_BFTreeRelease);
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
	void setCompare(int (*compare)(T & a, T & b)) {
		this->_compare = compare;
	}

	/**
	 * defines how objects are released
	 */
	void setRelease(void (*release)(T obj)) {
		this->_release = release;
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
		Container * c = new Container(object, this);
		return BFTreeInsert(this->_tree, c);
	}

	/**
	 * removes object from tree
	 */
	int remove(T object) {
		if (!this->_tree) return -1;
		Container c(object, this);
		return BFTreeRemove(this->_tree, &c);
	}

	/**
	 * true if tree contains object
	 */
	bool contains(T object) {
		if (!this->_tree) return -1;
		Container c(object, this);
		return BFTreeContains(this->_tree, &c);
	}

	/**
	 * returns root node
	 */
	const Node root() {
		return BFTreeGetRoot(this->_tree);
	}

private:

	/**
	 * compare: how each object is compared to each other
	 *	a < b -> result < 0 
	 *	a > b -> result > 0
	 *	a == b -> result == 0
	 */
	int (*_compare)(T & a, T & b);
	static int _BFTreeCompare(BFTreeObject a, BFTreeObject b) {
		Container * acont = (Container *) a;
		Container * bcont = (Container *) b;
		return acont->_treeRef->_compare(acont->_obj, bcont->_obj);
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

