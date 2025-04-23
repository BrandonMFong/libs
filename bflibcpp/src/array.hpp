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
template <typename T, typename S = size_t>
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
		
		this->deallocate(this->_address);
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
		this->_address = (T *) this->allocate(arr->count());
		this->_count = arr->count();
		memcpy(this->_address, arr->address(), this->_count);
	}

	/**
	 * copies content of arr to the end of ours
	 */
	void append(const Array<T,S> & arr) {
		this->_address = this->reallocate(this->_address, this->_count + arr._count);
		//memcpy(&this->_address[this->_count], &arr._address[0], arr._count);
		for (int i = this->_count; i < this->_count + arr._count; i++) {
			this->_address[i] = arr._address[i - this->_count];
		}
		this->_count += arr._count;
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
		this->_count = size;
		this->_address = this->reallocate(this->_address, this->_count);
	}

	/**
	 * Returns address of array
	 */
	T * address() const { return this->_address; }
	
private:

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
	void saveArray(const T * array, S size) {
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

template <> struct Sort<Array> {
	template <typename T, typename S = size_t> 
	int operator()(Array<T,S> & array, SortStrategy strategy = kSortStrategyMerge) {
		switch (strategy) {
			case kSortStrategyBubble:
				return Sort::sortBubble<T,S>(array);
			case kSortStrategyInsertion:
				return Sort::sortInsertion<T,S>(array);
			case kSortStrategySelection:
				return Sort::sortSelection<T,S>(array);
			case kSortStrategyQuick:
				return Sort::sortQuick<T,S>(array);
			case kSortStrategyMerge:
			default:
				return Sort::sortMerge<T,S>(array);
		}
	}

	/** BUBBLE SORT - START **/

	template <typename T, typename S = size_t> 
	static int sortBubble(Array<T,S> & c) {
		S n = c.size();
		bool swapped = false;

		for (S i = 0; i < n - 1; i++) {
			swapped = false;
			for (S j = 0; j < n - i - 1; j++) {
				if (c[j] > c[j + 1]) {
					swap(c[j], c[j + 1]);
					swapped = true;
				}
			}

			// If no two elements were swapped, then break
			if (!swapped)
				break;
		}
		return 0;
	}

	/** BUBBLE SORT - END **/
	/** INSERTION SORT - START **/

	template <typename T, typename S = size_t> 
	static int sortInsertion(Array<T,S> & c) {
		S i, j;
		T key;
		S n = c.size();
		for (i = 1; i < n; i++) {
			key = c[i];
			j = i - 1;
	 
			// Move elements of arr[0..i-1],
			// that are greater than key, to one
			// position ahead of their
			// current position
			while (j >= 0 && c[j] > key) {
				c[j + 1] = c[j];
				j = j - 1;
			}
			c[j + 1] = key;
		}
		return 0;
	}

	/** INSERTION SORT - END **/
	/** SELECTION SORT - START **/

	template <typename T, typename S = size_t> 
	static int sortSelection(Array<T,S> & c) {
		S i, j, min_idx;
		S n = c.size();

		// One by one move boundary of
		// unsorted subarray
		for (i = 0; i < n-1; i++) {
			// Find the minimum element in
			// unsorted array
			min_idx = i;
			for (j = i+1; j < n; j++)
				if (c[j] < c[min_idx])
					min_idx = j;

			// Swap the found minimum element
			// with the first element
			if (c[min_idx] != c[i]) {
				swap(c[min_idx], c[i]);
			}
		}
		return 0;
	}

	/** SELECTION SORT - END **/
	/** QUICK SORT - START **/

	template <typename T, typename S = size_t> 
	static int sortQuick(Array<T,S> & c) {
		return sortQuick<T,S>(c, 0, c.size() - 1);
	}

	template <typename T, typename S = size_t> 
	static int sortQuick(Array<T,S> & c, S low, S high) {
		if (low < high) {
			// pi is the partition return index of pivot
			S pi = sortQuickPartition<T,S>(c, low, high);

			// Recursion calls for smaller elements
			// and greater or equals elements
			sortQuick<T,S>(c, low, pi - 1);
			sortQuick<T,S>(c, pi + 1, high);
		}

		return 0;
	}

	template <typename T, typename S = size_t> 
	static int sortQuickPartition(Array<T,S> & c, S low, S high) {
		// Choose the pivot
		T pivot = c[high];
	  
		// Index of smaller element and indicates 
		// the right position of pivot found so far
		S i = low - 1;

		// Traverse arr[;ow..high] and move all smaller
		// elements on left side. Elements from low to 
		// i are smaller after every iteration
		for (S j = low; j <= high - 1; j++) {
			if (c[j] < pivot) {
				i++;
				if (c[i] != c[j]) {
					swap(c[i], c[j]);
				}
			}
		}
		
		// Move pivot after smaller elements and
		// return its position
		if (c[i + 1] != c[high]) {
			swap(c[i + 1], c[high]);
		}
		return i + 1;
	}

	/** QUICK SORT - END **/
	/** MERGE SORT - START **/

	template <typename T, typename S = size_t> 
	static int sortMerge(Array<T,S> & c) {
		return sortMerge<T,S>(c, 0, c.size() - 1);
	}

	template <typename T, typename S = size_t> 
	static int sortMerge(Array<T,S> & c, S left, S right) {
		if (left >= right)
			return 0;

		S mid = left + (right - left) / 2;
		sortMerge<T,S>(c, left, mid);
		sortMerge<T,S>(c, mid + 1, right);
		sortMerge<T,S>(c, left, mid, right);
		return 0;
	}

	template <typename T, typename S = size_t> 
	static int sortMerge(Array<T,S> & c, S left, S mid, S right) {
		S n1 = mid - left + 1;
		S n2 = right - mid;

		// Create temp vectors
		T L[n1], R[n2];

		// Copy data to temp vectors L[] and R[]
		for (S i = 0; i < n1; i++)
			L[i] = c[left + i];
		for (S j = 0; j < n2; j++)
			R[j] = c[mid + 1 + j];

		S i = 0, j = 0;
		S k = left;

		// Merge the temp vectors back
		// into arr[left..right]
		while (i < n1 && j < n2) {
			if (L[i] <= R[j]) {
				c[k] = L[i];
				i++;
			}
			else {
				c[k] = R[j];
				j++;
			}
			k++;
		}

		// Copy the remaining elements of L[],
		// if there are any
		while (i < n1) {
			c[k] = L[i];
			i++;
			k++;
		}

		// Copy the remaining elements of R[],
		// if there are any
		while (j < n2) {
			c[k] = R[j];
			j++;
			k++;
		}
		return 0;
	}
	
	/** MERGE SORT - END **/
};


} // namespace BF

#endif // ARRAY_HPP

