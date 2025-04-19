/**
 * author: brando
 * date: 11/27/24
 */

#ifndef HASH_MAP_HPP
#define HASH_MAP_HPP

#include "basicmap.hpp"
#include "release.hpp"
#include "retain.hpp"
#include "hash.hpp"

extern "C" {
#include <bflibc/hashmap.h>
}

namespace BF {

/**
 * Map implemented using self-balancing tree. See bflibc/map.h
 */
template <typename K, typename V, typename S = size_t, typename H = BF::Hash<K>>
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
	
	[[deprecated("Use BF::Hash to define hashing for your object")]]
	void setHash(unsigned long (*hash)(K & key)) { }

private:
	size_t size() const {
		return BFHashMapGetSize(this->_map);
	}

	virtual int _insert(void * key, void * value) {
		if (!this->_map) return -1;
		return BFHashMapInsert(this->_map, key, value);
	}

	virtual typename BasicMap<K,V,S>::template Value<V> * _getValueForKey(void * key) const {
		if (!this->_map) {
			return NULL;
		}
		
		typename BasicMap<K,V,S>::template Value<V> * value = 
			(typename BasicMap<K,V,S>::template Value<V> *) BFHashMapGetValue(this->_map, key, NULL);

		return value;
	}

	virtual int _remove(void * key) {
		if (!this->_map) return -1;
		return BFHashMapRemove(this->_map, key);
	}

	virtual bool _contains(void * key) const {
		if (!this->_map) return false;
		return BFHashMapContains(this->_map, key);
	}
	
	static unsigned long _BFHashMapHashFunction(void * k) {
		typename BasicMap<K,V,S>::template Key<K> * key = (typename BasicMap<K,V,S>::template Key<K> *) k;
		if (!key) {
			return 0;
		}
		
		HashMap * map = (HashMap *) key->_mapRef;
		if (!map) {
			return 0;
		}

		K obj = key->_obj;
		return map->_hash(obj);
	}

	//unsigned long (*_hash)(K & key);
	H _hash;

	BFHashMap _map;
};
}

#endif // MAP_HPP

