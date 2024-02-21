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
		this->_islocked = false;
	}

	// initializes with object
	Atomic(T obj) : Atomic() {
		this->set(obj);
	}

	~Atomic() {
		BFLockDestroy(&this->_objlock);
	}

	void unsafeset(T obj) {
		this->_obj = obj; 
	}

	T & unsafeget() {
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
	T & get() {
		BFLockLock(&this->_objlock);
		T & res = this->unsafeget();
		BFLockUnlock(&this->_objlock);
		return res;
	}

	void lock() {
		BFLockLock(&this->_objlock);
		this->_islocked = true;
	}

	void unlock() {
		this->_islocked = false;
		BFLockUnlock(&this->_objlock);
	}

	Atomic <T> & operator=(const T & obj) {
		this->set(obj);
		return *this;
	}

private:
	T _obj;
	bool _islocked;
	BFLock _objlock;
};

}

#endif // ATOMIC_HPP

