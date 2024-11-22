/**
 * author: Brando
 * date: 11/22/24
 */

#ifndef TREE_TESTS_H
#define TREE_TESTS_H

#include "clib_tests.h"
#include "tree.h"

BFTEST_UNIT_FUNC(test_treeinit, 1, {
	BFTree * tree = BFTreeCreate();
	BF_ASSERT(tree, "a null tree was returned");
	BFTreeRelease(tree);
})

BFTEST_COVERAGE_FUNC(tree_tests, {
	BFTEST_LAUNCH(test_treeinit);
})

#endif // TREE_TESTS_H

