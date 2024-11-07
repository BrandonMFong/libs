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

int test_collectionListSortDefault() {
	UNIT_TEST_START;
	int result = 0;
	int max = 1;

	while (!result && max--) {
		int maxsize = 2 << 4;
		srand(time(0));
		List<int> l;
		for (int i = 0; i < maxsize; i++) {
			int val = rand();
			l.add(val);
		}

		result = Collection<int>::sort((Collection<int>*) &l);
	}

	UNIT_TEST_END(!result, result);
	return result;
}

void collection_tests(int * pass, int * fail) {
	int p = 0, f = 0;
	
	INTRO_TEST_FUNCTION;

	LAUNCH_TEST(test_collectionListSortDefault, p, f);
	
	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // COLLECTION_TESTS_HPP

