/**
 * author: brando
 * date: 5/27/25
 */

#ifndef SET_HPP
#define SET_HPP

#include "basicset.hpp"
#include "release.hpp"

extern "C" {
#include <bflibc/set.h>
}

namespace BF {

/**
 * Set implemented using self-balancing tree. See bflibc/map.h
 */
template <typename V, typename S = size_t>
class Set : public BasicSet<V,S> {
public:
	Set() : _set(NULL), BasicSet<V,S>() {
		this->_set = BFSetCreate();
		if (!this->_set) return;
		BFSetSetCompare(this->_set, this->_BFSetCompare);
		BFSetSetRelease(this->_set, this->_BFSetRelease);
	}

	virtual ~Set() {
		BFSetRelease(this->_set);
	}

private:
	size_t size() const {
		return BFSetGetSize(this->_set);
	}

	virtual int _insert(void * value) {
		if (!this->_set) return -1;
		return BFSetInsert(this->_set, value);
	}

	virtual int _remove(void * value) {
		if (!this->_set) return -1;
		return BFSetRemove(this->_set, value);
	}

	virtual bool _contains(void * value) const {
		if (!this->_set) return false;
		return BFSetContains(this->_set, value);
	}

	BFSet _set;
};
}

#endif // SET_HPP

