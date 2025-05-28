/**
 * author: brando
 * date: 5/27/25
 */

#ifndef BASIC_SET_HPP
#define BASIC_SET_HPP

#include "collection.hpp"
#include "release.hpp"
#include "retain.hpp"
#include "exception.hpp"
#include "compare.hpp"
#include "allocator.hpp"

#include <type_traits>

namespace BF {

/**
 * General implementation of a Map
 *
 * This is formatted to fit the bflibc implementation of
 * HashMap and Map
 */
template <
	typename V,
	typename S = size_t,
	class C = Compare<V>
>
class BasicSet : public Collection<S> {
public:
	virtual const char * className() const {
		return "BF::BasicSet";
	}

protected:

	/**
	 * template on how Key and Value objects are
	 * held
	 *
	 * this object will get injected into the
	 * map implementations
	 */
	template<typename T, class A = Allocator<T>>
	class Container : public Object {
	public:
		T _obj;
		const BasicSet<V,S> * _setRef;
		Container(T obj, const BasicSet<V,S> * mapRef)
		: _obj(obj), _setRef(mapRef), Object() { }
		virtual ~Container() {
			A allocator;
			allocator.release(this->_obj);
		}
	};

public:
	BasicSet() : Collection<S>() { }

	virtual ~BasicSet() { }

	/**
	 * adds key and value into map
	 */
	int insert(V value) {
		Container<V> * v = new Container<V>(value, this);
		return this->_insert(v);
	}

	/**
	 * removes key/value pair with key
	 */
	int remove(V value) {
		Container<V> v(value, this);
		return this->_remove(&v);
	}

	/**
	 * true if there is an entry with key=k
	 */
	bool contains(V value) const {
		Container<V> v(value, this);
		return this->_contains(&v);
	}

private:
	virtual int _insert(void * value) = 0;
	virtual int _remove(void * value) = 0;
	virtual bool _contains(void * value) const = 0;

protected:
	/**
	 * compares a with b
	 *
	 * we assume similar behavior to strcmp or memcmp
	 *
	 * default return(-1)
	 */
	static int _BFSetCompare(void * a, void * b) {
		Container<V> * aval = (Container<V> *) a;
		Container<V> * bval = (Container<V> *) b;
		if (!aval || !bval) {
			return 0;
		}

		C cmp;
		return cmp(aval->_obj, bval->_obj);
	}

	/**
	 * Releases the Key & Value objects
	 */
 	static void _BFSetRelease(void * v) {
		Container<V> * value = (Container<V> *) v;
		BFRelease(value);
	}
};
}

#endif // BASIC_SET_HPP

