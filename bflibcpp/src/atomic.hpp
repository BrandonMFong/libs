/**
 * author: brando
 * date: 1/24/24
 */

#ifndef ATOMIC_HPP
#define ATOMIC_HPP

#include "access.hpp"
#include "object.hpp"
#include <stdbool.h>

extern "C" {
#include <bflibc/lock.h>
}

namespace BF {

template <typename T>
class Atomic : public Object {
public:
	Atomic() : Object() {
		BFLockCreate(&this->_objlock);
	}

	// initializes with object
	Atomic(T obj) : Atomic() {
		this->set(obj);
	}

	~Atomic() {
		if (this->_objlock) {
			BFLockDestroy(&this->_objlock);
			this->_objlock = NULL;
		}
	}

	void unsafeset(T obj) {
		this->_obj = obj; 
	}

	T & unsafeget() const {
		return this->_obj; 
	}

	void set(T obj) {
		BFLockLock(&this->_objlock);
		this->unsafeset(obj);
		BFLockUnlock(&this->_objlock);
	}

	// returns a reference to object
	//
	// caller does NOT own
	T & get() const {
		BFLockLock(&this->_objlock);
		T & res = this->unsafeget();
		BFLockUnlock(&this->_objlock);
		return res;
	}

	/**
	 * use unsafe getter and setter to access after
	 * this call
	 */
	void lock() const {
		BFLockLock(&this->_objlock);
	}

	/**
	 * must call after lock()
	 */
	void unlock() const {
		BFLockUnlock(&this->_objlock);
	}

	Atomic <T> & operator=(const T & obj) {
		this->set(obj);
		return *this;
	}

	Atomic <T> & operator=(const Atomic<T> & a) {
		this->set(a._obj);
		return *this;
	}

	friend bool operator==(const Atomic<T> & a, const Atomic<T> & b) {
		return a._obj == b._obj;
	}
	
	friend bool operator!=(const Atomic<T> & a, const Atomic<T> & b) {
		return a._obj != b._obj;
	}

	friend bool operator==(const Atomic<T> & a, const T & b) {
		return a._obj == b;
	}

	friend bool operator!=(const Atomic<T> & a, const T & b) {
		return a._obj != b;
	}
	
	operator T () const {
		return this->get();
	}

private:
	mutable T _obj;
	BFLock _objlock;
};

}

#endif // ATOMIC_HPP

