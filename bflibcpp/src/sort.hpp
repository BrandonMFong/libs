/**
 * author: brando
 * date: 4/23/25
 */

#ifndef SORT_HPP
#define SORT_HPP

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
}

#endif // SORT_HPP

