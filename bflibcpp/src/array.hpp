/**
 * author: Brando
 * date: 6/30/22
 */

#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <stdbool.h>
#include <stdint.h>
#include <initializer_list>
#include <utility>
#include <iostream>
#include "access.hpp"
#include "vector.hpp"
#include <string.h>
#include "exception.hpp"
#include "swap.hpp"

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
template <typename T, typename S = long>
class Array : public Vector<T,S> {
public:
	virtual const char * className() const {
		return "BF::Array";
	}

	Array() : Vector<T,S>() {
		this->_address = 0;
		this->_count = 0;
		this->_callback = Array::comparisonDefault;
		this->_releasecb = NULL;
		
		this->_capacity = 0;
	   	this->_blockSize = 2 << 3;
		this->allocate(*this, this->_blockSize);
	}

	/**
	 * Initializes with array
	 *
	 * array gets copied
	 */
	Array(T * array, S size) : Array() {
		this->set(array, size);	
	}

	/**
	 * Initializes with initializer
	 *
	 * list gets copied
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
		
		//this->deallocate(this->_address);
		this->deallocate(*this);
		this->_address = 0;
		this->_count = 0;
	}
	
	/**
	 * Initializes with array
	 */
	void set(const T * array, S size) {
		this->saveArray(array, size);	
	}

	/**
	 * Initializes with initializer
	 */
	void set(std::initializer_list<T> list) {
		this->saveArray(list);
	}

	/**
	 * Returns false if argument could not be found
	 *
	 * This function uses the _callback comparison 
	 * function to compare each function
	 */
	virtual bool contains(T object) const {
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
	virtual T objectAtIndex(S index) const {
		if ((this->_address == 0) || (this->_count == 0)) {
			return (T) 0;
		} else if (index >= this->_count) {
			return (T) 0;
		} else {
			return this->_address[index];
		}
	}

	/**
	 * returns reference to the object
	 *
	 * throws if index is out of range
	 */
	virtual T & refObjectAtIndex(S index) {
		if ((this->_address == 0) || (this->_count == 0)) {
			throw Exception("indexing null array");
		} else if (index >= this->_count) {
			throw Exception("index %d out of range", index);
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

	virtual S size() const {
		return this->count();
	}

	/**
	 * returns max object in list
	 *
	 * throws if count is 0, caller must check
	 */
	virtual T max() const {
		if (this->count() == 0) {
			throw Exception("cannot get max from empty array");
		}
		T res = this->objectAtIndex(0);
		for (S i = 1; i < this->count(); i++) {
			T tmp = this->objectAtIndex(i);
			if (res < tmp) {
				res = tmp;
			}
		}

		return res;
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
	void copyFromArray(const Array<T,S> * arr) {
		this->removeAll();
		this->allocate(*this, arr->count());
		this->_count = arr->count();
		memcpy(this->_address, arr->address(), this->_count);
	}

	/**
	 * copies content of arr to the end of ours
	 */
	void append(const Array<T,S> & arr) {
		this->reallocate(*this, this->_count, this->_count + arr._count);
		for (int i = this->_count; i < this->_count + arr._count; i++) {
			this->_address[i] = arr._address[i - this->_count];
		}
		this->_count += arr._count;
	}

	/**
	 * Adds object at the end of the array
	 */
	int add(T obj) {
		this->reallocate(*this, this->_count, this->_count + 1);
		if (this->_address == NULL) {
			this->_count = 0;
			return -3;
		}

		this->_count++;
		this->_address[this->_count - 1] = obj;
		return 0;
	}

	int insertObjectAtIndex(T obj, S index) {
		this->adjustMemorySize(this->_count+1);
		if (this->_address == NULL) {
			this->_count = 0;
			return -4;
		}

		// shift
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
		this->adjustMemorySize(this->_count-1);

		// if count == 0, then realloc will return NULL
		if (this->_count && (this->_address == NULL)) {
			this->_count = 0;
			return -5;
		}

		return 0;
	}

protected:

	/**
	 * adjusts address memory to size
	 */
	void adjustMemorySize(S size) {
		this->reallocate(*this, this->_count, size);
		this->_count = size;
	}

	/**
	 * Returns address of array
	 */
	T * address() const {
		if (this->_count == 0) return NULL;
		return this->_address;
	}
	
private:

	/**
	 * uses malloc to allocate mem
	 */
	static void allocate(Array<T,S> & array, S size) {
		if (size > array._capacity) {
			array._capacity = size;
			array._address = (T *) new T[size];
		}
	}

	/**
	 * returns modified `addr` with `newsize`
	 */
	static void reallocate(Array<T,S> & array, S oldsize, S newsize) {
		if (newsize < array._capacity) {
			return;
		}

		S adjustNewSize = (((newsize / array._blockSize) + 1) * array._blockSize);
		array._capacity = adjustNewSize;
		T * res = new T[adjustNewSize];
		for (S i = 0; i < oldsize && i < adjustNewSize; i++) {
			res[i] = std::move(array._address[i]);
			array._address[i] = 0;
		}

		delete[] array._address;
		array._address = res;

		return;
	}

	/**
	 * Derived must make sure this follows the standard established
	 * by allocate()
	 */
	//static void deallocate(T * value) {
	static void deallocate(Array<T,S> & array) {
		delete[] array._address;
		array._capacity = 0;
		//delete[] value;
	}

	/**
	 * Copies values from array
	 */
	void saveArray(const T * array, S size) {
		this->removeAll();
		this->allocate(*this, size);
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
		this->allocate(*this, this->_count);

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
	 * will hold a certain amount of reserved space for the
	 * array elements. we will ask for more memory when _count > _capacity
	 */
	S _capacity;

	/**
	 * block size of memory allocation
	 *
	 * each reallocation where count > capacity, will always
	 * allocate memory in blocks of blockSize
	 */
	S _blockSize;

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

public:

	void operator=(const std::initializer_list<T> & list) {
		this->saveArray(list);
	}

	/**
	 * Copies the string content from arr to us
	 */
	virtual Array<T,S> & operator=(const Array<T,S> & arr) {
		this->copyFromArray(&arr);
		return *this;
	}

	virtual T operator[](S index) const {
		return this->objectAtIndex(index);
	}

	virtual T & operator[](S index) {
		return this->refObjectAtIndex(index);
	}

// Comparators
public:
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

