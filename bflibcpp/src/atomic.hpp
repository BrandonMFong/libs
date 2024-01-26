/**
 * author: brando
 * date: 1/24/24
 */

#ifndef ATOMIC_HPP
#define ATOMIC_HPP

#include "access.hpp"
#include <pthread.h>
#include <stdbool.h>
#include <mutex>

namespace BF {

template <typename T>
class Atomic {
public:
	Atomic() {
		this->_locked = false;
	}

	// initializes with object
	Atomic(T obj) : Atomic() {
		this->set(obj);
	}

	~Atomic() {
	}

	void set(T obj) {
		std::lock_guard<std::mutex> l(this->_mut);
		this->_obj = obj; 
	}

	// returns a reference to object
	//
	// caller does NOT own
	T & get() {
		std::lock_guard<std::mutex> l(this->_mut);
		return this->_obj;
	}

	void lock() {
		std::lock_guard<std::mutex> l(this->_mut);
		this->_locked = true;
	}

	void unlock() {
		this->_locked = false;
		this->_mut.unlock();
	}

	Atomic <T> & operator=(const T & obj) {
		this->set(obj);
		return *this;
	}

private:
	T _obj;
	std::mutex _mut;
	bool _locked;
};

}

#endif // ATOMIC_HPP

