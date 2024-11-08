/**
 * author: brando
 * date: 11/7/24
 */

#ifndef COLLECTION_HPP
#define COLLECTION_HPP

#include "object.hpp"

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
	virtual ~Collection() {
		this->_subscriptIndex = 0;
	}

	virtual T objectAtIndex(S index) const = 0;
	virtual T & refObjectAtIndex(S index) = 0;
	virtual void replaceObjectAtIndex(T obj, S index)  = 0;

	int sort(CollectionSort type = kCollectionSortMerge) {
		switch (type) {
			case kCollectionSortMerge:
			default:
				return Collection::sortMerge(*this);
				break;
		}
	}

	T operator[](S index) const {
		return this->objectAtIndex(index);
	}

	T & operator[](S index) {
		return this->refObjectAtIndex(index);
	}

protected:
	Collection() : Object() { }

private:
	S _subscriptIndex;

	/** MERGE SORT - START **/

	static int sortMerge(Collection & c) {
		return 0;
	}

	static int sortMerge(Collection & c, S left, S right) {
		if (left >= right)
			return 0;

		S mid = left + (right - left) / 2;
		mergeSort(c, left, mid);
		mergeSort(c, mid + 1, right);
		mergeSort(c, left, mid, right);
		return 0;
	}

	static int sortMerge(Collection & c, S left, S mid, S right) {
		/*
		int n1 = mid - left + 1;
		int n2 = right - mid;

		// Create temp vectors
		vector<int> L(n1), R(n2);

		// Copy data to temp vectors L[] and R[]
		for (int i = 0; i < n1; i++)
			L[i] = arr[left + i];
		for (int j = 0; j < n2; j++)
			R[j] = arr[mid + 1 + j];

		int i = 0, j = 0;
		int k = left;

		// Merge the temp vectors back
		// into arr[left..right]
		while (i < n1 && j < n2) {
			if (L[i] <= R[j]) {
				arr[k] = L[i];
				i++;
			}
			else {
				arr[k] = R[j];
				j++;
			}
			k++;
		}

		// Copy the remaining elements of L[],
		// if there are any
		while (i < n1) {
			arr[k] = L[i];
			i++;
			k++;
		}

		// Copy the remaining elements of R[],
		// if there are any
		while (j < n2) {
			arr[k] = R[j];
			j++;
			k++;
		}
		*/
		return 0;
	}
	
	/** MERGE SORT - END **/
};
}

#endif // COLLECTION_HPP

