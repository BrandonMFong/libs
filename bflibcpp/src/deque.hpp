/**
 * author: brando
 * date: 4/17/25
 */

#ifndef DEQUE_HPP
#define DEQUE_HPP

#include "access.hpp"
#include "list.hpp"

namespace BF {

template <typename T, typename S = int>
class Deque : protected List<T,S> {
public:
	Deque() : List<T,S>() { }

	~Deque() { }

	int push_front(T object) {
		return this->insertObjectAtIndex(object, 0);
	}

	int push_back(T object) {
		return this->add(object);
	}

	int pop_front() {
		if (this->empty()) return -1;
		return this->deleteNode(this->first());
	}
	
	int pop_back() {
		if (this->empty()) return -1;
		return this->deleteNode(this->last());
	}

	T front() const {
		typename List<T,S>::Node * n = this->first();
		if (n) return n->object();
		return 0;
	}

	T back() const {
		typename List<T,S>::Node * n = this->last();
		if (n) return n->object();
		return 0;
	}

	S size() const { return this->count(); }

	bool empty() const { return this->count() == 0; };
};

} // namespace BF

#endif // DEQUE_HPP

