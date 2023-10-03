/**
 * author: Brando
 * date: 11/3/22
 */

#ifndef STACK_HPP
#define STACK_HPP

#include "access.hpp"
#include "list.hpp"

namespace BF {

/**
 * Has similar interface to cpp stack object
 *
 * This object does not handle any memory deallocation. Caller is still 
 * responsible for maintaining every object memory that gets pushed 
 * and popped. Thus it MUST be noted all references to objects will 
 * be deleted when stack is still full when stack object gets deleted
 */
template <typename T, typename S = int>
class Stack : protected List<T, S> {
public:
	Stack() : List<T,S>() {

	}

	~Stack() {

	}

	int push(T object) {
		return this->add(object);
	}

	int pop() {
		return this->deleteNode(this->last());
	}

	T top() {
		typename List<T,S>::Node * n = this->last();
		if (n) return n->object();
		return 0;
	}

	S size() { return this->count(); }

	bool empty() { return this->count() == 0; };
};

} // namespace BF

#endif // STACK_HPP

