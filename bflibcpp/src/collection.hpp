/**
 * author: brando
 * date: 11/7/24
 */

#ifndef COLLECTION_HPP
#define COLLECTION_HPP

#include "object.hpp"
#include <vector>

extern "C" {
#include <bflibc/swap.h>
}

namespace BF {

typedef enum {
	kCollectionSortBubble = 1,
	kCollectionSortInsertion = 2,
	kCollectionSortSelection = 3,
	kCollectionSortMerge = 4,
	kCollectionSortQuick = 5,
	kCollectionSortRadix = 6,
} CollectionSort;

/**
 * This class serves as a base class to all array type
 * implementations. Purpose is to abstract each class such
 * that I can have a set of mutable functions like sorting
 * to be used across different classes
 */
template <typename T, typename S = size_t>
class Collection : public Object {
public:
	virtual ~Collection() { }

	virtual T objectAtIndex(S index) const = 0;
	virtual T & refObjectAtIndex(S index) = 0;
	virtual void replaceObjectAtIndex(T obj, S index)  = 0;
	virtual S size() const = 0;

	T operator[](S index) const {
		return this->objectAtIndex(index);
	}

	T & operator[](S index) {
		return this->refObjectAtIndex(index);
	}

	int sort(CollectionSort type = kCollectionSortMerge) {
		switch (type) {
			case kCollectionSortBubble:
				return Collection::sortBubble(*this);
			case kCollectionSortInsertion:
				return Collection::sortInsertion(*this);
			case kCollectionSortSelection:
				return Collection::sortSelection(*this);
			case kCollectionSortMerge:
			default:
				return Collection::sortMerge(*this);
		}
	}

protected:
	Collection() : Object() { }

private:
	
	/** BUBBLE SORT - START **/

	static int sortBubble(Collection & c) {
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

	static int sortInsertion(Collection & c) {
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

	static int sortSelection(Collection & c) {
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
	/** MERGE SORT - START **/

	static int sortMerge(Collection & c) {
		return sortMerge(c, 0, c.size() - 1);
	}

	static int sortMerge(Collection & c, S left, S right) {
		if (left >= right)
			return 0;

		S mid = left + (right - left) / 2;
		sortMerge(c, left, mid);
		sortMerge(c, mid + 1, right);
		sortMerge(c, left, mid, right);
		return 0;
	}

	static int sortMerge(Collection & c, S left, S mid, S right) {
		int n1 = mid - left + 1;
		int n2 = right - mid;

		// Create temp vectors
		std::vector<int> L(n1), R(n2);

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

#endif // COLLECTION_HPP

