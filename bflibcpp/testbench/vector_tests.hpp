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

int test_vectorListSortBubble() {
	UNIT_TEST_START;
	int result = run_vectorSort<List<int>>(kVectorSortBubble, 2 << 9, 1);
	UNIT_TEST_END(!result, result);
	return result;
}

int test_vectorListSortInsertion() {
	UNIT_TEST_START;
	int result = run_vectorSort<List<int>>(kVectorSortInsertion, 2 << 10, 1);
	UNIT_TEST_END(!result, result);
	return result;
}

int test_vectorListSortSelection() {
	UNIT_TEST_START;
	int result = run_vectorSort<List<int>>(kVectorSortSelection, 2 << 9, 1);
	UNIT_TEST_END(!result, result);
	return result;
}

int test_vectorListSortQuick() {
	UNIT_TEST_START;
	int result = run_vectorSort<List<int>>(kVectorSortQuick, 2 << 9, 1);
	UNIT_TEST_END(!result, result);
	return result;
}

int test_vectorListSortMerge() {
	UNIT_TEST_START;
	int result = run_vectorSort<List<int>>(kVectorSortMerge, 2 << 12, 1);
	UNIT_TEST_END(!result, result);
	return result;
}

int test_vectorListSortRadix() {
	UNIT_TEST_START;
	int result = run_vectorSort<List<int>>(kVectorSortRadix, 2 << 10, 1);
	UNIT_TEST_END(!result, result);
	return result;
}

int test_vectorArraySortBubble() {
	UNIT_TEST_START;
	int result = run_vectorSort<Array<int>>(kVectorSortBubble, 2 << 9, 1);
	UNIT_TEST_END(!result, result);
	return result;
}

int test_vectorArraySortInsertion() {
	UNIT_TEST_START;
	int result = run_vectorSort<Array<int>>(kVectorSortInsertion, 2 << 10, 1);
	UNIT_TEST_END(!result, result);
	return result;
}

int test_vectorArraySortSelection() {
	UNIT_TEST_START;
	int result = run_vectorSort<Array<int>>(kVectorSortSelection, 2 << 9, 1);
	UNIT_TEST_END(!result, result);
	return result;
}

int test_vectorArraySortQuick() {
	UNIT_TEST_START;
	int result = run_vectorSort<Array<int>>(kVectorSortQuick, 2 << 9, 1);
	UNIT_TEST_END(!result, result);
	return result;
}

int test_vectorArraySortMerge() {
	UNIT_TEST_START;
	int result = run_vectorSort<Array<int>>(kVectorSortMerge, 2 << 12, 1);
	UNIT_TEST_END(!result, result);
	return result;
}

int test_vectorArraySortRadix() {
	UNIT_TEST_START;
	int result = run_vectorSort<Array<int>>(kVectorSortRadix, 2 << 10, 1);
	UNIT_TEST_END(!result, result);
	return result;
}

void vector_tests(int * pass, int * fail) {
	int p = 0, f = 0;
	
	INTRO_TEST_FUNCTION;

	/*
	LAUNCH_TEST(test_vectorListSortBubble, p, f);
	LAUNCH_TEST(test_vectorListSortInsertion, p, f);
	LAUNCH_TEST(test_vectorListSortSelection, p, f);
	LAUNCH_TEST(test_vectorListSortQuick, p, f);
	LAUNCH_TEST(test_vectorListSortMerge, p, f);
	LAUNCH_TEST(test_vectorListSortRadix, p, f);
	LAUNCH_TEST(test_vectorArraySortBubble, p, f);
	LAUNCH_TEST(test_vectorArraySortInsertion, p, f);
	LAUNCH_TEST(test_vectorArraySortSelection, p, f);
	LAUNCH_TEST(test_vectorArraySortQuick, p, f);
	LAUNCH_TEST(test_vectorArraySortMerge, p, f);
	*/
	LAUNCH_TEST(test_vectorArraySortRadix, p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // VECTOR_TESTS_HPP

