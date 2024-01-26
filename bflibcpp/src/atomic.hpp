/**
 * author: brando
 * date: 1/24/24
 */

#ifndef ATOMIC_HPP
#define ATOMIC_HPP

#include "access.hpp"
#include <pthread.h>
#include <stdbool.h>

namespace BF {

template <typename T>
class Atomic {
public:
	Atomic() {
		pthread_mutex_init(&this->_mut, 0);
	}

	Atomic(T obj) : Atomic() {
		this->set(obj);
	}

	~Atomic() {
		pthread_mutex_destroy(&this->_mut);
	}

	void set(T obj) {
		if (!this->_locked) this->lock();
		this->_obj = obj; 
		if (!this->_locked) this->unlock();
	}

	T get() {
		if (!this->_locked) this->lock();
		T res = this->_obj; 
		if (!this->_locked) this->unlock();
		return res;
	}

	void lock() {
		this->_locked = true;
		pthread_mutex_lock(&this->_mut);
	}

	void unlock() {
		this->_locked = false;
		pthread_mutex_unlock(&this->_mut);
	}

private:
	T _obj;
	pthread_mutex_t _mut;
	bool _locked;
};

}

#endif // ATOMIC_HPP

