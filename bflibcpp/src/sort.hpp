/**
 * author: brando
 * date: 4/23/25
 */

#ifndef SORT_HPP
#define SORT_HPP

#include "list.hpp"

namespace BF {

typedef enum {
	kSortStrategyBubble = 1,
	kSortStrategyInsertion = 2,
	kSortStrategySelection = 3,
	kSortStrategyMerge = 4,
	kSortStrategyQuick = 5,
} SortStrategy;

template<template <typename...> class T> struct Sort;

}

#endif // SORT_HPP

