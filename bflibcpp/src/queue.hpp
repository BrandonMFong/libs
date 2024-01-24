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
class Queue : public List<T,S> {
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
};

} // namespace BF

#endif // QUEUE_HPP

