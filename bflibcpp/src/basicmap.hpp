/**
 * author: brando
 * date: 11/26/24
 */

#ifndef BASIC_MAP_HPP
#define BASIC_MAP_HPP

#include "collection.hpp"
#include "release.hpp"

extern "C" {
#include <bflibc/map.h>
}

namespace BF {

/**
 * General implementation of a Map
 *
 * This is formatted to fit the bflibc implementation of
 * HashMap and Map
 */
template <typename K, typename V, typename S = size_t>
class BasicMap : public Collection<S> {
protected:

	/**
	 * template on how Key and Value objects are
	 * held
	 *
	 * this object will get injected into the
	 * map implementations
	 */
	template<typename T>
	class Container : public Object {
	public:
		T _obj;
		BasicMap<K,V,S> * _mapRef;
		Container(T obj, BasicMap<K,V,S> * mapRef)
		: _obj(obj), _mapRef(mapRef), Object() {
			BFRetain(this->_mapRef);
		}
		virtual ~Container() {
			BFRelease(this->_mapRef);
		}
	};

	template<typename T>
	class Key : public Container<T> {
	public:
		Key(T obj, BasicMap<K,V,S> * mapRef)
		: Container<T>(obj, mapRef) { }
		virtual ~Key() {
			if (this->_mapRef->_releaseKey) {
				this->_mapRef->_releaseKey(this->_obj);
			}
		}
	};

	template<typename T>
	class Value : public Container<T> {
	public:
		Value(T obj, BasicMap<K,V,S> * mapRef)
		: Container<T>(obj, mapRef) { }
		virtual ~Value() {
			if (this->_mapRef->_releaseValue) {
				this->_mapRef->_releaseValue(this->_obj);
			}
		}
	};

	friend class Key<K>;
	friend class Value<V>;

public:
	BasicMap()
	: _compare(NULL), _releaseKey(NULL),
	_releaseValue(NULL), Collection<S>() { }

	virtual ~BasicMap() { }

	/**
	 * compare callback for keys
	 *
	 * similar behavior to strcmp and memcmp
	 */
	void setCompare(int (*compare)(K & a, K & b)) {
		this->_compare = compare;
	}

	/**
	 * defines how Key and values are released
	 */
	void setRelease(void (*releaseKey)(K obj), void (*releaseValue)(V obj)) {
		this->_releaseKey = releaseKey;
		this->_releaseValue = releaseValue;
	}

	/**
	 * adds key and value into map
	 */
	int insert(K k, V v) {
		Key<K> * key = new Key<K>(k, this);
		Value<V> * value = new Value<V>(v, this);
		return this->_insert(key, value);
	}

	/**
	 * returns value for key with optional error
	 */
	V & getValueForKey(K k, int * error) {
		Key<K> key(k, this);

		int err = 0;
		Value<V> * value = this->_getValueForKey(&key, &err);
		if (!value || err != 0) {
			throw Exception("Couldn't get value for key");
		}
		if (error) *error = err;

		return value->_obj;
	}

	/**
	 * returns value for key
	 */
	V & at(K k) {
		this->getValueForKey(k, NULL);
	}

	/**
	 * removes key/value pair with key
	 */
	int remove(K k) {
		Key<K> key(k, this);
		return this->_remove(&key);
	}

	/**
	 * true if there is an entry with key=k
	 */
	bool contains(K k) {
		Key<K> key(k, this);
		return this->_contains(&key);
	}

private:
	virtual int _insert(void * key, void * value) = 0;
	virtual Value<V> * _getValueForKey(void * key, int * error) = 0;
	virtual int _remove(void * key) = 0;
	virtual bool _contains(void * key) = 0;

	int (*_compare)(K & a, K & b);
	void (*_releaseKey)(K obj);
	void (*_releaseValue)(V obj);

protected:
	/**
	 * compares a with b
	 *
	 * we assume similar behavior to strcmp or memcmp
	 *
	 * default return(-1)
	 */
	static int _BFMapCompare(void * a, void * b) {
		Key<K> * akey = (Key<K> *) a;
		Key<K> * bkey = (Key<K> *) b;
		if (!akey || !bkey || !akey->_mapRef->_compare) {
			return -1;
		}
		return akey->_mapRef->_compare(akey->_obj, bkey->_obj);
	}

	/**
	 * Releases the Key & Value objects
	 */
 	static void _BFMapRelease(void * k, void * v) {
		Key<K> * key = (Key<K> *) k;
		Value<V> * value = (Value<V> *) v;
		BFRelease(key);
		BFRelease(value);
	}
};
}

#endif // BASIC_MAP_HPP

