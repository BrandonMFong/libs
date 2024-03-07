/**
 * author: Brando
 * date: 6/30/22
 */

#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <stdbool.h>
#include <stdint.h>
#include <initializer_list>
#include <iostream>
#include "delete.hpp"
#include "access.hpp"
#include "object.hpp"
#include <string.h>

namespace BF {

/**
 * Mutable Array
 *
 * This class allows you to access the array.  This class 
 * is meant to provide you the basic functionality of an array class
 *
 * Objects stored in array are assumed to be owned by owner of array 
 * object
 */
template <typename T, typename S = size_t> class Array : public Object {
PUBLIC:
	Array() : Object() {
		this->_address = 0;
		this->_count = 0;
		this->_callback = Array::comparisonDefault;
		this->_releasecb = NULL;
	}

	/**
	 * Initializes with array
	 */
	Array(T * array, S size) : Array() {
		this->set(array, size);	
	}

	/**
	 * Initializes with initializer
	 */
	Array(std::initializer_list<T> list) : Array() {
		this->set(list);
	}

	virtual ~Array() {
		this->removeAll();
	}

	void removeAll() {
		// if there is a release callback
		// then let's call this
		//
		// this callback handles the memory of each element. This
		// is set by the owner of this object
		if (this->_releasecb) {
			for (S i = 0; i < this->_count; i++) {
				this->_releasecb((this->_address)[i]);
			}
		}
		
		this->deallocate(this->_address);
		this->_address = 0;
		this->_count = 0;
	}
	
	/**
	 * Initializes with array
	 */
	void set(T * array, S size) {
		this->saveArray(array, size);	
	}

	/**
	 * Initializes with initializer
	 */
	void set(std::initializer_list<T> list) {
		this->saveArray(list);
	}

	/**
	 * Establishes how allocation works
	 *
	 * By default free store is utilized
	 */
	[[deprecated("allocation is no longer configurable")]]
	void setAllocationCallback(T * (* cb) (S size)) {
		//this->_allocationCallback = cb;
	}

	/**
	 * Establishes how deallocation works
	 *
	 * By default free store is utilized
	 */
	[[deprecated("allocation is no longer configurable")]]
	void setDeallocationCallback(void (* cb) (T * value)) {
		//this->_deallocationCallback = cb;
	}

	/**
	 * Returns false if argument could not be found
	 *
	 * This function uses the _callback comparison 
	 * function to compare each function
	 */
	virtual bool contains(T object) {
		for (S i = 0; i < this->_count; i++) {
			if (	this->_callback((this->_address)[i], object) 
				== 	0) 
				return true;
		}
		return false;
	}

	/**
	 * Returns null if argument could not be found
	 */
	T objectAtIndex(S index) const {
		if ((this->_address == 0) || (this->_count == 0)) {
			return (T) 0;
		} else if (index >= this->_count) {
			return (T) 0;
		} else {
			return this->_address[index];
		}
	}

	/**
	 * Returns the value's index. If we could not find it, -1 will be returned
	 *
	 * This will return the first match
	 */
	S indexForObject(T object) const {
		for (S i = 0; i < this->_count; i++) {
			if (	this->_callback(this->_address[i], object)
				== 	0)
				return i;
		}
		return -1;
	}

	/// Returns _count
	virtual S count() const {
		return this->_count;
	}

	/**
	 * Prints the array from the first element to the last
	 */
	virtual void print() {
		std::cout << "[ ";
		for (S i = 0; i < this->_count; i++) {
			std::cout << this->_address[i];
			std::cout << " ";
		}
		std::cout << "]" << std::endl;
	}

	/**
	 * Rename comparator to compare callback
	 */
	void setComparator(int (* callback) (T a, T b)) {
		this->_callback = callback;
	}

	/**
	 * sets release callback
	 */
	void setReleaseCallback(void (* callback) (T obj)) {
		this->_releasecb = callback;
	}

	/**
	 * Copies content from arr to us
	 */
	void copyFromArray(const Array<T> * arr) {
		this->removeAll();
		this->_address = (T *) this->allocate(arr->count());
		this->_count = arr->count();
		memcpy(this->_address, arr->address(), this->_count);
	}

	/**
	 * Adds object at the end of the array
	 */
	int add(T obj) {
		this->_address = this->reallocate(this->_address, this->_count + 1);
		if (this->_address == NULL) {
			this->_count = 0;
			return -3;
		}

		this->_count++;
		this->_address[this->_count - 1] = obj;
		return 0;
	}

	int insertObjectAtIndex(T obj, S index) {
		this->_address = this->reallocate(this->_address, this->_count + 1);
		if (this->_address == NULL) {
			this->_count = 0;
			return -4;
		}

		// shift
		this->_count++;
		for (S i = this->_count - 1; i > index; i--) {
			this->_address[i] = this->_address[i - 1];
		}

		this->_address[index] = obj;

		return 0;
	}

	/**
	 * removes object at index
	 *
	 * this dynamically adjusts the memory
	 */
	int removeObjectAtIndex(S index) {
		// shift objects
		for (S i = index; (i+1) < this->_count; i++) {
			this->_address[i] = this->_address[i + 1];
		}

		// adjust array
		this->_count--;
		this->_address = this->reallocate(this->_address, this->_count);

		// if count == 0, then realloc will return NULL
		if (this->_count && (this->_address == NULL)) {
			this->_count = 0;
			return -5;
		}

		return 0;
	}

PROTECTED:

	/**
	 * Returns address of array
	 */
	T * address() const { return this->_address; }
	
PRIVATE:

	/**
	 * uses malloc to allocate mem
	 */
	static T * allocate(S size) {
		return (T *) malloc(sizeof(T) * size);
	}

	/**
	 * returns modified `addr` with `newsize`
	 */
	static T * reallocate(T * addr, S newsize) {
		return (T *) realloc(addr, sizeof(T) * newsize);
	}

	/**
	 * Derived must make sure this follows the standard established
	 * by allocate()
	 */
	static void deallocate(T * value) {
		free((void *) value);
	}

	/**
	 * Copies values from array
	 */
	void saveArray(T * array, S size) {
		this->removeAll();
		this->_address = (T *) this->allocate(size);
		this->_count = size;

		if (this->_address) {
			// Load into array
			for (S i = 0; i < size; i++) {
				this->_address[i] = array[i];
			}
		}
	}

	/**
	 * Sweeps through the initializer list to set out array's memory
	 */
	void saveArray(std::initializer_list<T> list) {
		this->removeAll();
		typename std::initializer_list<T>::iterator itr;

		this->_count = list.size();
		this->_address = (T *) this->allocate(this->_count);

		if (this->_address) {
			S i = 0;
			for (itr = list.begin(); itr != list.end(); ++itr) {
				this->_address[i] = *itr;
				i++;
			}
		}
	}

	/** 
	 * Holds copy of array
	 *
	 * Use this to hold the address of where the data will be
	 */
	T * _address;

	/// Holds size of _address
	S _count;

	/**
	 * How we compare each item in the array
	 */
	int (* _callback) (T a, T b);

	/**
	 * this will determine how each
	 * element is released
	 *
	 * this is set to null by default
	 */
	void (* _releasecb) (T obj);

PUBLIC:

	T operator[](S index) const {
		return this->objectAtIndex(index);
	}

	void operator=(const std::initializer_list<T> & list) {
		this->saveArray(list);
	}

	/**
	 * Copies the string content from arr to us
	 */
	Array<T> & operator=(const Array<T> & arr) {
		this->copyFromArray(&arr);
		return *this;
	}

// Comparators
PUBLIC:
	/**
	 * Compares the raw value of a and b
	 */
	static int comparisonDefault(T a, T b) {
		if (a < b) {
			return -1;
		} else if (a > b) {
			return 1;
		} else {
			return 0;
		}
	}
};

} // namespace BF

#endif // ARRAY_HPP

