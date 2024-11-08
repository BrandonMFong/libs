/**
 * author: Brando
 * date: 11/7/24
 */

#ifndef COLLECTION_TESTS_HPP
#define COLLECTION_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include <list.hpp>

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

int run_collectionListSort(CollectionSort type, int listsize, int reps) {
	int result = 0;
	int max = reps;

	while (!result && max--) {
		int maxsize = listsize;
		srand(time(0));
		List<int> l;
		for (int i = 0; i < maxsize; i++) {
			int val = rand();
			l.add(val);
		}

		result = l.sort(type);

		if (!result) {
			if (l.size() != maxsize) {
				result = 2;
			}
		}

		if (!result) {
			for (int i = 1; i < maxsize; i++) {
				if (l[i - 1] > l[i]) {
					result = 1;
					break;
				}
			}
		}
	}

	return result;
}

int test_collectionListSortBubble() {
	UNIT_TEST_START;
	int result = run_collectionListSort(kCollectionSortBubble, 2 << 8, 1);
	UNIT_TEST_END(!result, result);
	return result;
}

int test_collectionListSortMerge() {
	UNIT_TEST_START;
	int result = run_collectionListSort(kCollectionSortMerge, 2 << 12, 1);
	UNIT_TEST_END(!result, result);
	return result;
}

void collection_tests(int * pass, int * fail) {
	int p = 0, f = 0;
	
	INTRO_TEST_FUNCTION;

	LAUNCH_TEST(test_collectionListSortBubble, p, f);
	LAUNCH_TEST(test_collectionListSortMerge, p, f);
	
	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // COLLECTION_TESTS_HPP

