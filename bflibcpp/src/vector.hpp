/**
 * author: brando
 * date: 11/7/24
 */

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "collection.hpp"

namespace BF {

/**
 * This class serves as a base class to all array type
 * implementations. Purpose is to abstract each class such
 * that I can have a set of mutable functions like sorting
 * to be used across different classes
 */
template <typename T, typename S = size_t>
class Vector : public Collection<S> {
public:
	virtual ~Vector() { }

	virtual T objectAtIndex(S index) const = 0;
	virtual T & refObjectAtIndex(S index) = 0;
	virtual T max() const = 0;

	virtual T operator[](S index) const {
		return this->objectAtIndex(index);
	}

	virtual T & operator[](S index) {
		return this->refObjectAtIndex(index);
	}

protected:
	Vector() : Collection<S>() { }
};
}

#endif // VECTOR_HPP

