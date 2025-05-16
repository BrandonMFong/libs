/**
 * author: Brando
 * date: 5/14/25
 */

#ifndef SET_TESTS_H
#define SET_TESTS_H

#include "clib_tests.h"
#include "set.h"

BFTEST_UNIT_FUNC(test_setinit, 2<<10, {
	BFSet set = BFSetCreate();
	BF_ASSERT(set, "null set");
	BFSetRelease(set);
})

int BFTestSetCompareInteger(BFSetValue aobj, BFSetValue bobj) {
	int a = (intptr_t) aobj;
	int b = (intptr_t) bobj;
	return a - b;
}

BFTEST_UNIT_FUNC(test_setcontains, 2<<10, {
	BFSet set = BFSetCreate();
	BF_ASSERT(set, "null set");

	BFSetSetCompare(set, BFTestSetCompareInteger);

	int max = 2 << 9;
	for (int i = 0; i < max; i++) {
		if (i % 2 == 0) {
			BF_ASSERT(BFSetInsert(set, (BFSetValue) (intptr_t) i) == 0, "could not insert %d", i);
		}
	}

	for (int i = 0; i < max; i++) {
		BF_ASSERT(BFSetContains(set, (BFSetValue) (intptr_t) i) == (i % 2 == 0 ? true : false), "contains(%d) != %s", i, (i % 2 == 0 ? "true" : "false"));
	}
	
	BFSetRelease(set);
})

BFTEST_UNIT_FUNC(test_setsize, 2<<10, {
	BFSet set = BFSetCreate();
	BF_ASSERT(set, "null set");

	BFSetSetCompare(set, BFTestSetCompareInteger);

	srand(time(0));
	int max = rand() % 2<<9;
	for (int i = 0; i < max; i++) {
		BF_ASSERT(BFSetInsert(set, (BFSetValue) (intptr_t) i) == 0, "could not insert %d", i);
	}

	BF_ASSERT(BFSetGetSize(set) == max, "size(%ld) != %ld", BFSetGetSize(set), max);
	BFSetRelease(set);
})

BFTEST_COVERAGE_FUNC(set_tests, {
	BFTEST_LAUNCH(test_setinit);
	BFTEST_LAUNCH(test_setcontains);
	BFTEST_LAUNCH(test_setsize);
})

#endif // SET_TESTS_H

