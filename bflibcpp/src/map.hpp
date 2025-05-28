/**
 * author: brando
 * date: 11/26/24
 */

#ifndef MAP_HPP
#define MAP_HPP

#include "basicmap.hpp"
#include "release.hpp"

extern "C" {
#include <bflibc/map.h>
}

namespace BF {

/**
 * Map implemented using self-balancing tree. See bflibc/map.h
 */
template <typename K, typename V, typename S = size_t>
class Map : public BasicMap<K,V,S> {
public:
	Map() : _map(NULL), BasicMap<K,V,S>() {
		this->_map = BFMapCreate();
		if (!this->_map) return;
		BFMapSetCompare(this->_map, this->_BFMapCompare);
		BFMapSetRelease(this->_map, this->_BFMapRelease);
	}

	virtual ~Map() {
		BFMapRelease(this->_map);
	}

private:
	size_t size() const {
		return BFMapGetSize(this->_map);
	}

	virtual int _insert(void * key, void * value) {
		if (!this->_map) return -1;
		return BFMapInsert(this->_map, key, value);
	}

	virtual typename BasicMap<K,V,S>::template Container<V> * _getValueForKey(void * key) const {
		if (!this->_map) {
			return NULL;
		}
		
		typename BasicMap<K,V,S>::template Container<V> * value = 
			(typename BasicMap<K,V,S>::template Container<V> *) BFMapGetValue(this->_map, key, NULL);

		return value;
	}

	virtual int _remove(void * key) {
		if (!this->_map) return -1;
		return BFMapRemove(this->_map, key);
	}

	virtual bool _contains(void * key) const {
		if (!this->_map) return false;
		return BFMapContains(this->_map, key);
	}

	BFMap _map;
};
}

#endif // MAP_HPP

