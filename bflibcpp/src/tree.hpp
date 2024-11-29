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
	class Node : public Object {
		BFTreeNode _node;
	public:
		Node(BFTreeNode node) : _node(node), Object() { }
		virtual ~Node() { }
		Node left() { return BFTreeNodeGetLeft(this->_node); }
		Node right() { return BFTreeNodeGetRight(this->_node); }
		T & object() {
			Container * cont = (Container *) BFTreeNodeGetObject(this->_node);
			if (!cont) {
				throw Exception("object is null");
			}
			return cont->_obj;
		}
		bool isNull() { return this->_node == NULL; }
	};

	Tree() : Collection<S>() {
		this->_tree = BFTreeCreate();
		if (!this->_tree) return;
		BFTreeSetCompare(this->_tree, this->_BFTreeCompare);
		BFTreeSetRelease(this->_tree, this->_BFTreeRelease);
	}

	void setCompare(int (*compare)(T & a, T & b)) {
		this->_compare = compare;
	}

	void setRelease(void (*release)(T obj)) {
		this->_release = release;
	}

	virtual ~Tree() {
		BFTreeRelease(this->_tree);
	}

	S size() const {
		return BFTreeSize(this->_tree);
	}

	int insert(T object) {
		if (!this->_tree) return -1;
		Container * c = new Container(object, this);
		return BFTreeInsert(this->_tree, c);
	}

	int remove(T object) {
		if (!this->_tree) return -1;
		Container c(object, this);
		return BFTreeRemove(this->_tree, &c);
	}

	bool contains(T object) {
		if (!this->_tree) return -1;
		Container c(object, this);
		return BFTreeContains(this->_tree, &c);
	}

	Node root() {
		return BFTreeGetRoot(this->_tree);
	}

private:
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

