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
#include <string.h>

namespace BF {

/**
 * Immutable Array
 *
 * This class allows you to access the array in readonly.  This class 
 * is meant to provide you the basic functionality of an array class
 *
 * Notes:
 * 	I feel that I can derive binary trees and linked lists from this class
 */
template <typename T, typename S = size_t> class Array {
PUBLIC:
	Array() {
		this->_address = 0;
		this->_count = 0;
		this->_callback = Array::comparisonDefault;
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
	 * Copies content from arr to us
	 */
	void copyFromArray(const Array<T> * arr) {
		this->removeAll();
		this->_address = (T *) this->allocate(arr->count());
		this->_count = arr->count();
		memcpy(this->_address, arr->address(), this->_count);
	}

PROTECTED:

	/**
	 * Returns address of array
	 */
	T * address() const { return this->_address; }
	
PRIVATE:

	/**
	 * Derived classes can override if they want to use the heap
	 *
	 * By default we are using the free store
	 */
	static T * allocate(S size) {
		return (T *) malloc(sizeof(T) * size);
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

PUBLIC:

	T operator[](S index) {
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

