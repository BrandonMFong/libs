/**
 * author: brando
 * date: 5/19/25
 */

#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

namespace BF {

template <typename T>
struct Allocator {
	T create() const {
		return 0;
	}

	void release(T obj) const { }
};

}

#endif // ALLOCATOR_HPP

