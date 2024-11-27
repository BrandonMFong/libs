/**
 * author: brando
 * date: 11/26/24
 */

#ifndef MAP_HPP
#define MAP_HPP

#include "object.hpp"
#include "release.hpp"

extern "C" {
#include <bflibc/map.h>
}

namespace BF {

template <typename K, typename V, typename S = int>
class Map : public Object {
	class Key : public Object {
	public:
		K _obj;
		Map<K,V,S> * _mapRef;
		Key(K obj, Map<K,V,S> * mapRef)
		: _obj(obj), _mapRef(mapRef), Object() {
			BFRetain(this->_mapRef);
		}
		virtual ~Key() {
			if (this->_mapRef->_releaseKey) {
				this->_mapRef->_releaseKey(this->_obj);
			}
			BFRelease(this->_mapRef);
		}
	};
	class Value : public Object {
	public:
		V _obj;
		Map<K,V,S> * _mapRef;
		Value(V obj, Map<K,V,S> * mapRef)
		: _obj(obj), _mapRef(mapRef), Object() {
			BFRetain(this->_mapRef);
		}
		virtual ~Value() {
			if (this->_mapRef->_releaseValue) {
				this->_mapRef->_releaseValue(this->_obj);
			}
			BFRelease(this->_mapRef);
		}
	};
	friend class Key;
	friend class Value;
public:
	Map()
	: _compare(NULL), _releaseKey(NULL),
	_releaseValue(NULL), _map(NULL), Object() {
		this->_map = BFMapCreate();
		if (!this->_map) return;
		BFMapSetCompare(this->_map, this->_BFMapCompare);
		BFMapSetRelease(this->_map, this->_BFMapRelease);
	}

	virtual ~Map() {
		BFMapRelease(this->_map);
	}

	size_t size() const {
		return BFMapGetSize(this->_map);
	}

	void setCompare(int (*compare)(K & a, K & b)) {
		this->_compare = compare;
	}

	void setRelease(void (*releaseKey)(K obj), void (*releaseValue)(V obj)) {
		this->_releaseKey = releaseKey;
		this->_releaseValue = releaseValue;
	}

	int insert(K k, V v) {
		if (!this->_map) return -1;
		Key * key = new Key(k, this);
		Value * value = new Value(v, this);
		return BFMapInsert(this->_map, key, value);
	}

	V getValueForKey(K k, int * error) {
		if (!this->_map) return -1;
		Key key(k, this);

		int err = 0;
		Value * value = (Value *) BFMapGetValue(this->_map, &key, &err);
		if (!value) {
			if (error) *error = err;
			return 0;
		}

		return value->_obj;
	}

	V at(K k) {
		this->getValueForKey(k, NULL);
	}

private:
	int (*_compare)(K & a, K & b);
	void (*_releaseKey)(K obj);
	void (*_releaseValue)(V obj);
	static int _BFMapCompare(BFMapKey a, BFMapKey b) {
		Key * akey = (Key *) a;
		Key * bkey = (Key *) b;
		if (!akey || !bkey || !akey->_mapRef->_compare) {
			return -1;
		}
		return akey->_mapRef->_compare(akey->_obj, bkey->_obj);
	}
 	static void _BFMapRelease(BFMapKey k, BFMapValue v) {
		Key * key = (Key *) k;
		Value * value = (Value *) v;
		BFRelease(key);
		BFRelease(value);
	}

	BFMap _map;
};
}

#endif // MAP_HPP

