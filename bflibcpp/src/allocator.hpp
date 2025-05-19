/**
 * author: brando
 * date: 5/19/25
 */

#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

namespace BF {

template <typename T>
struct Allocator {
	bool canRelease() const {
		return false;
	}

	bool canCreate() const {
		return false;
	}
};

}

#endif // ALLOCATOR_HPP

