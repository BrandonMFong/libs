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
		BFLockCreate(&this->_lock);
		this->_locked = false;
	}

	// initializes with object
	Atomic(T obj) : Atomic() {
		this->set(obj);
	}

	~Atomic() {
		BFLockDestroy(&this->_lock);
	}

	void set(T obj) {
		if (!this->_locked) BFLockLock(&this->_lock);
		this->_obj = obj; 
		if (!this->_locked) BFLockUnlock(&this->_lock);
	}

	// returns a reference to object
	//
	// caller does NOT own
	T & get() {
		if (!this->_locked) BFLockLock(&this->_lock);
		T & res = this->_obj;
		if (!this->_locked) BFLockUnlock(&this->_lock);
		return res;
	}

	void lock() {
		BFLockLock(&this->_lock);
		this->_locked = true;
	}

	void unlock() {
		this->_locked = false;
		BFLockUnlock(&this->_lock);
	}

	Atomic <T> & operator=(const T & obj) {
		this->set(obj);
		return *this;
	}

private:
	T _obj;
	bool _locked;
	BFLock _lock;
};

}

#endif // ATOMIC_HPP

