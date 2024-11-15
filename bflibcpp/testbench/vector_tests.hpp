/**
 * author: Brando
 * date: 11/7/24
 */

#ifndef VECTOR_TESTS_HPP
#define VECTOR_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include <list.hpp>

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

template <typename T>
int run_vectorSort(VectorSort type, size_t listsize, int reps) {
	int result = 0;
	int max = reps;
	while (!result && max--) {
		usleep(50);
		size_t maxsize = listsize;
		srand(time(0));
		T l;
		T b;
		for (size_t i = 0; i < maxsize; i++) {
			int val = rand();
			b.add(val);
			l.add(val);
		}

		result = l.sort(type);

		if (!result) {
			if (l.size() != maxsize) {
				result = 2;
			}
		}

		if (!result) {
			for (size_t i = 1; i < maxsize; i++) {
				if (l[i - 1] > l[i]) {
					result = 1;
					break;
				}
			}
			for (size_t i = 0; i < maxsize; i++) {
				if (!l.contains(b[i])) {
					result = 3;
				}
			}
		}
	}

	return result;
}

BFTEST_UNIT_FUNC(test_vectorListSortBubble, 1,  {
	int err = run_vectorSort<List<int>>(kVectorSortBubble, 2 << 9, 1);
	BF_ASSERT(err == 0);
})

BFTEST_UNIT_FUNC(test_vectorListSortInsertion, 1,  {
	int err = run_vectorSort<List<int>>(kVectorSortInsertion, 2 << 10, 1);
	BF_ASSERT(err == 0);
})

BFTEST_UNIT_FUNC(test_vectorListSortSelection, 1,  {
	int err = run_vectorSort<List<int>>(kVectorSortSelection, 2 << 9, 1);
	BF_ASSERT(err == 0);
})

BFTEST_UNIT_FUNC(test_vectorListSortQuick, 1,  {
	int err = run_vectorSort<List<int>>(kVectorSortQuick, 2 << 9, 1);
	BF_ASSERT(err == 0);
})

BFTEST_UNIT_FUNC(test_vectorListSortMerge, 1, {
	int err = run_vectorSort<List<int>>(kVectorSortMerge, 2 << 12, 1);
	BF_ASSERT(err == 0);
})

BFTEST_UNIT_FUNC(test_vectorArraySortBubble, 1,  {
	int err = run_vectorSort<Array<int>>(kVectorSortBubble, 2 << 12, 4);
	BF_ASSERT(err == 0);
})

BFTEST_UNIT_FUNC(test_vectorArraySortInsertion, 1,  {
	int err = run_vectorSort<Array<int>>(kVectorSortInsertion, 2 << 13, 4);
	BF_ASSERT(err == 0);
})

BFTEST_UNIT_FUNC(test_vectorArraySortSelection, 1,  {
	int err = run_vectorSort<Array<int>>(kVectorSortSelection, 2 << 13, 4);
	BF_ASSERT(err == 0);
})

BFTEST_UNIT_FUNC(test_vectorArraySortQuick, 1,  {
	int err = run_vectorSort<Array<int>>(kVectorSortQuick, 2 << 13, 4);
	BF_ASSERT(err == 0);
})

BFTEST_UNIT_FUNC(test_vectorArraySortMerge, 1,  {
	int err = run_vectorSort<Array<int>>(kVectorSortMerge, 2 << 12, 4);
	BF_ASSERT(err == 0);
})

BFTEST_COVERAGE_FUNC(vector_tests) {
	BFTEST_COVERAGE_START;

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

	BFTEST_COVERAGE_END;
}

#endif // VECTOR_TESTS_HPP

