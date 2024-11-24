/**
 * author: Brando
 * date: 11/22/24
 */

#ifndef MAP_TESTS_H
#define MAP_TESTS_H

#include "clib_tests.h"

BFTEST_UNIT_FUNC(test_mapinit, 1, {

})

BFTEST_COVERAGE_FUNC(map_tests, {
	BFTEST_LAUNCH(test_mapinit);
})

#endif // MAP_TESTS_H

