/**
 * author: brando
 * date: 11/26/24
 */

#ifndef BASIC_MAP_HPP
#define BASIC_MAP_HPP

#include "collection.hpp"
#include "release.hpp"
#include "retain.hpp"
#include "exception.hpp"
#include "compare.hpp"
#include "allocator.hpp"

#include <type_traits>

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
template <
	typename K,
	typename V,
	typename S = size_t,
	class C = Compare<K>,
	class AK = Allocator<K>,
	class AV = Allocator<V>
>
class BasicMap : public Collection<S> {
public:
	virtual const char * className() const {
		return "BF::BasicMap";
	}

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
		const BasicMap<K,V,S> * _mapRef;
		Container(T obj, const BasicMap<K,V,S> * mapRef)
		: _obj(obj), _mapRef(mapRef), Object() {
			BFRetain(this->_mapRef);
		}
		virtual ~Container() {
			BFRelease(this->_mapRef);
		}

		bool isBFObject() const {
			return std::is_base_of_v<BF::Object, T>;
		}
	};

	template<typename T>
	class Key : public Container<T> {
	public:
		Key(T obj, const BasicMap<K,V,S> * mapRef)
		: Container<T>(obj, mapRef) { }
		virtual ~Key() {
			/*
			if (this->_mapRef->_releaseKey) {
				this->_mapRef->_releaseKey(this->_obj);
			}
			*/
			AK allocator;
			allocator.release(this->_obj);
		}
	};

	template<typename T>
	class Value : public Container<T> {
	public:
		Value(T obj, const BasicMap<K,V,S> * mapRef)
		: Container<T>(obj, mapRef) { }
		virtual ~Value() {
			/*
			if (this->_mapRef->_releaseValue) {
				this->_mapRef->_releaseValue(this->_obj);
			}
			*/
			AV allocator;
			allocator.release(this->_obj);
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
	[[deprecated("Please use BF::Compare functor")]]
	void setCompare(int (*compare)(K & a, K & b)) { }

	/**
	 * defines how Key and values are released
	 */
	[[deprecated("Please use BF::Allocatorfunctor")]]
	void setRelease(void (*releaseKey)(K obj), void (*releaseValue)(V obj)) { }

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
	 *
	 * throws an exception if no value could be found for key
	 */
	V & getValueForKey(K k) const {
		Key<K> key(k, this);

		Value<V> * value = this->_getValueForKey(&key);
		if (value) {
			return value->_obj;
		} else {
			throw Exception("could not get value for key"); // TODO: display key in message
		}
	}

	/**
	 * returns value for key
	 *
	 * if no value is found, an exception is thrown
	 */
	V & at(K k) const {
		return this->getValueForKey(k);
	}

	V & operator[](K k) const {
		return this->at(k);
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
	bool contains(K k) const {
		Key<K> key(k, this);
		return this->_contains(&key);
	}

private:
	virtual int _insert(void * key, void * value) = 0;
	virtual int _remove(void * key) = 0;
	virtual bool _contains(void * key) const = 0;

	/**
	 * returns NULL if there is no value for key
	 */
	virtual Value<V> * _getValueForKey(void * key) const = 0;

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
		if (!akey || !bkey) {
			return 0;
		}
	
		/*
		if (!akey->_mapRef->_compare && akey->isBFObject() && bkey->isBFObject()) {
			BF::Object * obja = (BF::Object *) &akey->_obj;
			BF::Object * objb = (BF::Object *) &bkey->_obj;
			if (!obja || !objb) return -1;
			return obja->compare(*objb);
		}

		return akey->_mapRef->_compare(akey->_obj, bkey->_obj);
		*/

		C cmp;
		return cmp(akey->_obj, bkey->_obj);
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

