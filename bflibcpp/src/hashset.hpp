/**
 * author: brando
 * date: 5/27/25
 */

#ifndef HASH_SET_HPP
#define HASH_SET_HPP

#include "basicset.hpp"
#include "release.hpp"
#include "retain.hpp"
#include "hash.hpp"

extern "C" {
#include <bflibc/hashset.h>
}

namespace BF {

/**
 * Map implemented using self-balancing tree. See bflibc/map.h
 */
template <typename V, typename S = size_t, typename H = BF::Hash<V>>
class HashSet : public BasicSet<V,S> {
public:
	HashSet() : _set(NULL), BasicSet<V,S>() {
		this->_set = BFHashSetCreate();
		if (!this->_set) return;
		BFHashSetSetCompare(this->_set, this->_BFSetCompare);
		BFHashSetSetRelease(this->_set, this->_BFSetRelease);
		BFHashSetSetHashFunction(this->_set, this->_BFHashSetHashFunction);
	}

	virtual ~HashSet() {
		BFHashSetRelease(this->_set);
	}

private:
	size_t size() const {
		return BFHashSetGetSize(this->_set);
	}

	virtual int _insert(void * value) {
		if (!this->_set) return -1;
		return BFHashSetInsert(this->_set, value);
	}

	virtual int _remove(void * value) {
		if (!this->_set) return -1;
		return BFHashSetRemove(this->_set, value);
	}

	virtual bool _contains(void * value) const {
		if (!this->_set) return false;
		return BFHashSetContains(this->_set, value);
	}
	
	static unsigned long _BFHashSetHashFunction(void * value) {
		typename BasicSet<V,S>::template Container<V> * v = (typename BasicSet<V,S>::template Container<V> *) value;
		if (!v) {
			return 0;
		}
		
		HashSet * set = (HashSet *) v->_setRef;
		if (!set) {
			return 0;
		}

		V obj = v->_obj;
		return set->_hash(obj);
	}

	H _hash;
	BFHashSet _set;
};
}

#endif // MAP_HPP

