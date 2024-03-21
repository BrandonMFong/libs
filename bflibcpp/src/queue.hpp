/**
 * author: brando
 * date: 1/24/24
 */

#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "access.hpp"
#include "list.hpp"

namespace BF {

template <typename T, typename S = int>
class Queue : protected List<T,S> {
public:
	Queue() : List<T,S>() {

	}

	~Queue() {

	}

	int push(T object) {
		return this->add(object);
	}

	int pop() {
		return this->deleteNode(this->first());
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

	S size() { return this->count(); }

	bool empty() { return this->count() == 0; };
};

} // namespace BF

#endif // QUEUE_HPP

