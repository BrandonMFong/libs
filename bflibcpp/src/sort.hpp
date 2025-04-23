/**
 * author: brando
 * date: 4/23/25
 */

#ifndef SORT_HPP
#define SORT_HPP

#include "list.hpp"
#include "array.hpp"

namespace BF {

typedef enum {
	kSortStrategyBubble = 1,
	kSortStrategyInsertion = 2,
	kSortStrategySelection = 3,
	kSortStrategyMerge = 4,
	kSortStrategyQuick = 5,
} SortStrategy;

template<template <typename...> class T> struct Sort;

// Generic sort function
template <template <typename...> class ContainerTemplate, typename... Args>
int sort(ContainerTemplate<Args...>& v, SortStrategy strategy = kSortStrategyMerge) {
    Sort<ContainerTemplate> sorter; // Instantiate Sort with the container template
    return sorter(v);
}

template <> struct Sort<List> {
	template <typename L, typename S = size_t> 
	int operator()(List<L,S> & list, SortStrategy strategy = kSortStrategyMerge) {
		switch (strategy) {
			case kSortStrategyBubble:
				return Sort::sortBubble<L,S>(list);
			case kSortStrategyInsertion:
				return Sort::sortInsertion<L,S>(list);
			case kSortStrategySelection:
				return Sort::sortSelection<L,S>(list);
			case kSortStrategyQuick:
				return Sort::sortQuick<L,S>(list);
			case kSortStrategyMerge:
			default:
				return Sort::sortMerge<L,S>(list);
		}
	}

	/** BUBBLE SORT - START **/

	template <typename L, typename S = size_t> 
	static int sortBubble(List<L,S> & c) {
		typename List<L,S>::Node * head = c.first();
		S len = c.size();
		int itr = 0;
		bool swapped = false;

		while (itr < len) {
			typename List<L,S>::Node * trav = head;
			typename List<L,S>::Node * prev = head;
			swapped = false;
			
			while (trav->next()) {
				typename List<L,S>::Node * ptr = trav->next();
				if (trav->object() > ptr->object()) {
					swapped = true;
					if (trav == head) {
						trav->right = ptr->right;
						ptr->right = trav;
						prev = ptr;
						head = prev;
					} else {
						trav->right = ptr->right;
						ptr->right = trav;
						prev->right = ptr;
						prev = ptr;
					}

					continue;
				}
				prev = trav;
				trav = trav->next();
			}

			if (!swapped) {
				break;
			}
			++itr;
		}

		c._head = head;

		return 0;
	}

	/** BUBBLE SORT - END **/
	/** INSERTION SORT - START **/

	template <typename L, typename S = size_t> 
	static List<L,S>::Node * sortInsertion(typename List<L,S>::Node * newNode, typename List<L,S>::Node * sorted) {
		if (!sorted || sorted->object() >= newNode->object()) {
			newNode->right = sorted;
			sorted = newNode;
		} else {
			typename List<L,S>::Node * curr = sorted;
			while (curr->right && curr->right->object() < newNode->object()) {
				curr = curr->next();
			}

			newNode->right = curr->right;
			curr->right = newNode;
		}

		return sorted;
	}

	template <typename L, typename S = size_t> 
	static int sortInsertion(List<L,S> & c) {
		typename List<L,S>::Node * head = c.first();
		typename List<L,S>::Node * sorted = NULL;
		typename List<L,S>::Node * curr = head;

		while (curr) {
			typename List<L,S>::Node * next = curr->next();
			sorted = sortInsertion<L,S>(curr, sorted);
			curr = next;
		}

		c._head = sorted;

		return 0;
	}

	/** INSERTION SORT - END **/
	/** SELECTION SORT - START **/

	template <typename L, typename S = size_t> 
	static int sortSelection(List<L,S> & c) {
		typename List<L,S>::Node * head = c.first();
		for (typename List<L,S>::Node * start = head; start; start = start->next()) {
			typename List<L,S>::Node * min = start;
			for (typename List<L,S>::Node * curr = start->next(); curr; curr = curr->next()) {
				if (curr->object() < min->object()) {
					min = curr;
				}
			}

			if (min != start) {
				L val = start->object();
				start->obj = min->object();
				min->obj = val;
			}
		}

		c._head = head;
		return 0;
	}

	/** SELECTION SORT - END **/
	/** QUICK SORT - START **/

	template <typename L, typename S = size_t> 
	static int sortQuick(List<L,S> & c) {
		typename List<L,S>::Node * head = c.first();
		typename List<L,S>::Node * tail = c.last();

		sortQuick<L,S>(head, tail);

		c._head = head;
		return 0;
	}
	
	template <typename L, typename S = size_t> 
	static void sortQuick(typename List<L,S>::Node * head, typename List<L,S>::Node * tail) {
		if (!head || head == tail) {
			return;
		}

		typename List<L,S>::Node * pivot = sortQuickGetPivot<L,S>(head, tail);

		sortQuick<L,S>(head, pivot);
		sortQuick<L,S>(pivot->next(), tail);
	}
	
	template <typename L, typename S = size_t> 
	static List<L,S>::Node * sortQuickGetPivot(typename List<L,S>::Node * head, typename List<L,S>::Node * tail) {
		typename List<L,S>::Node * pivot = head;

		typename List<L,S>::Node * pre = head;
		typename List<L,S>::Node * curr = head;

		while (curr != tail->next()) {
			if (curr->object() < pivot->object()) {
				BF::swap<L>(curr->obj, pre->right->obj);
				pre = pre->next();
			}

			curr = curr->next();
		}

		BF::swap<L>(pivot->obj, pre->obj);

		return pre;
	}

	/** QUICK SORT - END **/
	/** MERGE SORT - START **/

	template <typename L, typename S = size_t> 
	static int sortMerge(List<L,S> & c) {
		c._head = sortMerge<L,S>(c._head);
		return 0;
	}

	template <typename L, typename S = size_t> 
	static List<L,S>::Node * sortMerge(typename List<L,S>::Node * head) {
		if (!head || !head->next()) {
			return head;
		}

		typename List<L,S>::Node * second = sortMergeSplit<L,S>(head);
		head = sortMerge<L,S>(head);
		second = sortMerge<L,S>(second);

		return sortMerge<L,S>(head, second);
	}
	
	template <typename L, typename S = size_t> 
	static List<L,S>::Node * sortMerge(typename List<L,S>::Node * first, typename List<L,S>::Node * second) {
		if (!first) return second;
		if (!second) return first;

		if (first->object() < second->object()) {
			first->right = sortMerge<L,S>(first->next(), second);
			return first;
		} else {
			second->right = sortMerge<L,S>(first, second->next());
			return second;
		}
	}

	template <typename L, typename S = size_t> 
	static List<L,S>::Node * sortMergeSplit(typename List<L,S>::Node * head) {
		typename List<L,S>::Node * fast = head;
		typename List<L,S>::Node * slow = head;

		while (fast && fast->next()) {
			fast = fast->right->right;
			if (fast) {
				slow = slow->right;
			}
		}

		typename List<L,S>::Node * tmp = slow->right;
		slow->right = NULL;
		return tmp;
	}
	
	/** MERGE SORT - END **/
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

}

#endif // SORT_HPP

