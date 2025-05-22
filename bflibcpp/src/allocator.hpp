/**
 * author: brando
 * date: 5/19/25
 */

#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

namespace BF {

template <typename T>
struct Allocator {
	bool canCreate() const {
		return false;
	}

	T create() const {
		return 0;
	}

	bool canRelease() const {
		return false;
	}

	void release(T obj) const { }
};

}

#endif // ALLOCATOR_HPP

