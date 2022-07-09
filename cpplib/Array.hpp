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

/**
 * Immutable Array
 *
 * This class allows you to access the array in readonly.  This class 
 * is meant to provide you the basic functionality of an array class
 *
 * Notes:
 * 	I feel that I can derive binary trees and linked lists from this class
 */
template <typename T> class Array {
public:
	Array() {
		this->_address = 0;
		this->_count = 0;
	}

	/**
	 * Initializes with array
	 */
	Array(T * array, uint64_t size) : Array() {
		this->set(array, size);	
	}

	/**
	 * Initializes with initializer
	 */
	Array(std::initializer_list<T> list) : Array() {
		this->set(list);
	}

	virtual ~Array() {
		if (this->_address) delete (T *) this->_address;
		this->_count = 0;
	}
	
	/**
	 * Initializes with array
	 */
	void set(T * array, uint64_t size) {
		this->_saveArray(array, size);	
	}

	/**
	 * Initializes with initializer
	 */
	void set(std::initializer_list<T> list) {
		this->_saveArray(list);
	}

	/**
	 * Returns false if argument could not be found
	 */
	virtual bool contains(T object) {
		for (uint64_t i = 0; i < this->_count; i++) {
			if (((T *) this->_address)[i] == object)
				return true;
		}
		return false;
	}

	/**
	 * Returns null if argument could not be found
	 */
	T objectAtIndex(uint64_t index) {
		if ((this->_address == 0) || (this->_count == 0)) {
			return (T) 0;
		} else {
			return ((T *) this->_address)[index];
		}
	}

	/**
	 * Returns the value's index. If we could not find it, -1 will be returned
	 *
	 * This will return the first match
	 */
	int64_t indexForObject(T value) {
		for (uint64_t i = 0; i < this->_count; i++) {
			if (((T *) this->_address)[i] == value) return i;
		}
		return -1;
	}

	/// Returns _count
	int count() {
		return (int) this->_count;
	}

	/**
	 * Prints the array from the first element to the last
	 */
	void print() {
		std::cout << "[ ";
		for (uint64_t i = 0; i < this->_count; i++) {
			std::cout << ((T *) this->_address)[i];
			std::cout << " ";
		}
		std::cout << "]" << std::endl;
	}

private:

	/**
	 * Copies values from array
	 */
	void _saveArray(T * array, uint64_t size) {
		this->_address = new T[size];
		this->_count = size;

		if (this->_address) {
			// Load into array
			for (uint64_t i = 0; i < size; i++) {
				((T *) this->_address)[i] = array[i];
			}
		}
	}

	/**
	 * Sweeps through the initializer list to set out array's memory
	 */
	void _saveArray(std::initializer_list<T> list) {
		typename std::initializer_list<T>::iterator itr;

		this->_count = list.size();
		this->_address = new T[this->_count];

		if (this->_address) {
			uint64_t i = 0;
			for (itr = list.begin(); itr != list.end(); ++itr) {
				((T *) this->_address)[i] = *itr;
				i++;
			}
		}
	}

	/** 
	 * Holds copy of array
	 *
	 * Use this to hold the address of where the data will be
	 */
	void * _address;

	/// Holds size of _address
	uint64_t _count;

public:

	T operator[](uint64_t index) {
		return this->objectAtIndex(index);
	}

	void operator=(const std::initializer_list<T> & list) {
		this->_saveArray(list);
	}
};

#endif // ARRAY_HPP

