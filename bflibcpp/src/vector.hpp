/**
 * author: brando
 * date: 11/7/24
 */

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "collection.hpp"
#include <vector>

extern "C" {
#include <bflibc/swap.h>
}

namespace BF {

typedef enum {
	kVectorSortBubble = 1,
	kVectorSortInsertion = 2,
	kVectorSortSelection = 3,
	kVectorSortMerge = 4,
	kVectorSortQuick = 5,
} VectorSort;

/**
 * This class serves as a base class to all array type
 * implementations. Purpose is to abstract each class such
 * that I can have a set of mutable functions like sorting
 * to be used across different classes
 */
template <typename T, typename S = size_t>
class Vector : public Collection<S> {
public:
	virtual ~Vector() { }

	virtual T objectAtIndex(S index) const = 0;
	virtual T & refObjectAtIndex(S index) = 0;
	virtual void replaceObjectAtIndex(T obj, S index)  = 0;
	virtual T max() const = 0;

	T operator[](S index) const {
		return this->objectAtIndex(index);
	}

	T & operator[](S index) {
		return this->refObjectAtIndex(index);
	}

	int sort(VectorSort type = kVectorSortMerge) {
		switch (type) {
			case kVectorSortBubble:
				return Vector::sortBubble(*this);
			case kVectorSortInsertion:
				return Vector::sortInsertion(*this);
			case kVectorSortSelection:
				return Vector::sortSelection(*this);
			case kVectorSortQuick:
				return Vector::sortQuick(*this);
			case kVectorSortMerge:
			default:
				return Vector::sortMerge(*this);
		}
	}

protected:
	Vector() : Collection<S>() { }

private:
	
	/** BUBBLE SORT - START **/

	static int sortBubble(Vector & c) {
		int n = c.size();
		bool swapped;

		for (int i = 0; i < n - 1; i++) {
			swapped = false;
			for (int j = 0; j < n - i - 1; j++) {
				if (c[j] > c[j + 1]) {
					BFSwap(c[j], c[j + 1]);
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

	static int sortInsertion(Vector & c) {
		int i, key, j;
		int n = c.size();
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

	static int sortSelection(Vector & c) {
		int i, j, min_idx;
		int n = c.size();

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
				BFSwap(c[min_idx], c[i]);
			}
		}
		return 0;
	}

	/** SELECTION SORT - END **/
	/** QUICK SORT - START **/

	static int sortQuick(Vector & c) {
		return sortQuick(c, 0, c.size() - 1);
	}

	static int sortQuick(Vector & c, int low, int high) {
		if (low < high) {
			// pi is the partition return index of pivot
			int pi = sortQuickPartition(c, low, high);

			// Recursion calls for smaller elements
			// and greater or equals elements
			sortQuick(c, low, pi - 1);
			sortQuick(c, pi + 1, high);
		}

		return 0;
	}

	static int sortQuickPartition(Vector & c, int low, int high) {
		// Choose the pivot
		int pivot = c[high];
	  
		// Index of smaller element and indicates 
		// the right position of pivot found so far
		int i = low - 1;

		// Traverse arr[;ow..high] and move all smaller
		// elements on left side. Elements from low to 
		// i are smaller after every iteration
		for (int j = low; j <= high - 1; j++) {
			if (c[j] < pivot) {
				i++;
				if (c[i] != c[j]) {
					BFSwap(c[i], c[j]);
				}
			}
		}
		
		// Move pivot after smaller elements and
		// return its position
		if (c[i + 1] != c[high]) {
			BFSwap(c[i + 1], c[high]);
		}
		return i + 1;
	}

	/** QUICK SORT - END **/
	/** MERGE SORT - START **/

	static int sortMerge(Vector & c) {
		return sortMerge(c, 0, c.size() - 1);
	}

	static int sortMerge(Vector & c, S left, S right) {
		if (left >= right)
			return 0;

		S mid = left + (right - left) / 2;
		sortMerge(c, left, mid);
		sortMerge(c, mid + 1, right);
		sortMerge(c, left, mid, right);
		return 0;
	}

	static int sortMerge(Vector & c, S left, S mid, S right) {
		int n1 = mid - left + 1;
		int n2 = right - mid;

		// Create temp vectors
		std::vector<T> L(n1), R(n2);

		// Copy data to temp vectors L[] and R[]
		for (int i = 0; i < n1; i++)
			L[i] = c[left + i];
		for (int j = 0; j < n2; j++)
			R[j] = c[mid + 1 + j];

		int i = 0, j = 0;
		int k = left;

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
}

#endif // VECTOR_HPP

