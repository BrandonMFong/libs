/**
 * author: brando
 * date: 11/27/24
 */

#ifndef HASH_MAP_HPP
#define HASH_MAP_HPP

#include "basicmap.hpp"
#include "release.hpp"
#include "exception.hpp"

extern "C" {
#include <bflibc/hashmap.h>
}

namespace BF {

/**
 * Map implemented using self-balancing tree. See bflibc/map.h
 */
template <typename K, typename V, typename S = size_t>
class HashMap : public BasicMap<K,V,S> {
public:
	HashMap() : _map(NULL), BasicMap<K,V,S>() {
		this->_map = BFHashMapCreate();
		if (!this->_map) return;
		BFHashMapSetCompare(this->_map, this->_BFMapCompare);
		BFHashMapSetRelease(this->_map, this->_BFMapRelease);
		BFHashMapSetHashFunction(this->_map, this->_BFHashMapHashFunction);
	}

	virtual ~HashMap() {
		BFHashMapRelease(this->_map);
	}

	void setHash(unsigned long (*hash)(K & key)) {
		this->_hash = hash;
	}

private:
	size_t size() const {
		return BFHashMapGetSize(this->_map);
	}

	virtual int _insert(void * key, void * value) {
		if (!this->_map) return -1;
		return BFHashMapInsert(this->_map, key, value);
	}

	virtual typename BasicMap<K,V,S>::template Value<V> * _getValueForKey(void * key, int * error) {
		if (!this->_map) {
			if (error) *error = -1;
			return NULL;
		}
		
		typename BasicMap<K,V,S>::template Value<V> * value = 
			(typename BasicMap<K,V,S>::template Value<V> *) BFHashMapGetValue(this->_map, key, error);
		if (!value) {
			if (error && *error != 0) *error = -1;
			return NULL;
		}

		return value;
	}

	virtual int _remove(void * key) {
		if (!this->_map) return -1;
		return BFHashMapRemove(this->_map, key);
	}

	virtual bool _contains(void * key) {
		if (!this->_map) return false;
		return BFHashMapContains(this->_map, key);
	}
	
	static unsigned long _BFHashMapHashFunction(void * k) {
		typename BasicMap<K,V,S>::template Key<K> * key = (typename BasicMap<K,V,S>::template Key<K> *) k;
		if (!key) {
			return 0;
		}
		HashMap * map = (HashMap *) key->_mapRef;
		if (!map->_hash) {
			return -1;
		}
		return map->_hash(key->_obj);
	}

	unsigned long (*_hash)(K & key);

	BFHashMap _map;
};
}

#endif // MAP_HPP

