/**
 * author: Brando
 * date: 11/7/24
 */

#ifndef VECTOR_TESTS_HPP
#define VECTOR_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include <list.hpp>
#include <array.hpp>
#include <sort.hpp>

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

template <typename T>
int run_vectorSortDefault(SortStrategy type, size_t listsize, int reps) {
	int result = 0;
	int max = reps;
	while (!result && max--) {
		usleep(50);
		size_t maxsize = listsize;
		srand(time(0));
		T l;
		T b;
		for (size_t i = 0; i < maxsize; i++) {
			usleep(50);
			int val = rand();
			b.add(val);
			l.add(val);
		}

		result = sort(l);

		if (!result) {
			if (l.size() != maxsize) {
				result = 2;
			}
		}

		if (!result) {
			for (size_t i = 1; i < maxsize; i++) {
				usleep(50);
				if (l[i - 1] > l[i]) {
					result = 1;
					break;
				}
			}
			for (size_t i = 0; i < maxsize; i++) {
				usleep(50);
				if (!l.contains(b[i])) {
					result = 3;
				}
			}
		}
	}

	return result;
}

template <template <typename...> class ContainerTemplate, typename... Args>
int run_vectorSort(SortStrategy type, size_t listsize, int reps) {
	int result = 0;
	int max = reps;
	while (!result && max--) {
		usleep(50);
		size_t maxsize = listsize;
		srand(time(0));
		ContainerTemplate<Args...> l;
		ContainerTemplate<Args...> b;
		for (size_t i = 0; i < maxsize; i++) {
			usleep(50);
			int val = rand();
			b.add(val);
			l.add(val);
		}

		Sort<ContainerTemplate<Args...>> sorter;
		result = sorter(l, type);

		if (!result) {
			if (l.size() != maxsize) {
				result = 2;
			}
		}

		if (!result) {
			for (size_t i = 1; i < maxsize; i++) {
				usleep(50);
				if (l[i - 1] > l[i]) {
					result = 1;
					break;
				}
			}
			for (size_t i = 0; i < maxsize; i++) {
				usleep(50);
				if (!l.contains(b[i])) {
					result = 3;
				}
			}
		}
	}

	return result;
}

BFTEST_UNIT_FUNC(test_vectorListSortBubble, 1,  {
	int err = run_vectorSort<List, int>(kSortStrategyBubble, 2 << 9, 1);
	BF_ASSERT(err == 0);
})

BFTEST_UNIT_FUNC(test_vectorListSortInsertion, 1,  {
	int err = run_vectorSort<List, int>(kSortStrategyInsertion, 2 << 9, 1);
	BF_ASSERT(err == 0);
})

BFTEST_UNIT_FUNC(test_vectorListSortSelection, 1,  {
	int err = run_vectorSort<List, int>(kSortStrategySelection, 2 << 9, 1);
	BF_ASSERT(err == 0);
})

BFTEST_UNIT_FUNC(test_vectorListSortQuick, 1,  {
	int err = run_vectorSort<List, int>(kSortStrategyQuick, 2 << 9, 1);
	BF_ASSERT(err == 0);
})

BFTEST_UNIT_FUNC(test_vectorListSortMerge, 1, {
	int err = run_vectorSort<List, int>(kSortStrategyMerge, 2 << 9, 1);
	BF_ASSERT(err == 0);
})

BFTEST_UNIT_FUNC(test_vectorArraySortBubble, 1,  {
	int err = run_vectorSort<Array, int>(kSortStrategyBubble, 2 << 9, 4);
	BF_ASSERT(err == 0);
})

BFTEST_UNIT_FUNC(test_vectorArraySortInsertion, 1,  {
	int err = run_vectorSort<Array, int>(kSortStrategyInsertion, 2 << 9, 4);
	BF_ASSERT(err == 0);
})

BFTEST_UNIT_FUNC(test_vectorArraySortSelection, 1,  {
	int err = run_vectorSort<Array, int>(kSortStrategySelection, 2 << 9, 4);
	BF_ASSERT(err == 0);
})

BFTEST_UNIT_FUNC(test_vectorArraySortQuick, 1,  {
	int err = run_vectorSort<Array, int>(kSortStrategyQuick, 2 << 9, 4);
	BF_ASSERT(err == 0);
})

BFTEST_UNIT_FUNC(test_vectorArraySortMerge, 1,  {
	int err = run_vectorSort<Array, int>(kSortStrategyMerge, 2 << 9, 4);
	BF_ASSERT(err == 0);
})

BFTEST_UNIT_FUNC(test_vectorArraySortDefault, 1,  {
	int err = run_vectorSortDefault<Array<int>>(kSortStrategyMerge, 2 << 9, 4);
	BF_ASSERT(err == 0);
})

BFTEST_UNIT_FUNC(test_vectorListSortDefault, 1,  {
	int err = run_vectorSortDefault<Array<int>>(kSortStrategyMerge, 2 << 9, 4);
	BF_ASSERT(err == 0);
})

BFTEST_COVERAGE_FUNC(vector_tests, {
	BFTEST_LAUNCH(test_vectorListSortBubble);
	BFTEST_LAUNCH(test_vectorListSortInsertion);
	BFTEST_LAUNCH(test_vectorListSortSelection);
	BFTEST_LAUNCH(test_vectorListSortQuick);
	BFTEST_LAUNCH(test_vectorListSortMerge);
	BFTEST_LAUNCH(test_vectorArraySortBubble);
	BFTEST_LAUNCH(test_vectorArraySortInsertion);
	BFTEST_LAUNCH(test_vectorArraySortSelection);
	BFTEST_LAUNCH(test_vectorArraySortQuick);
	BFTEST_LAUNCH(test_vectorArraySortMerge);
	BFTEST_LAUNCH(test_vectorArraySortDefault);
	BFTEST_LAUNCH(test_vectorListSortDefault);

})

#endif // VECTOR_TESTS_HPP

