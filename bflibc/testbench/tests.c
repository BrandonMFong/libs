/**
 * author: Brando
 * date: 6/30/22
 */

#include "coreutils_tests.h"
#include "filesystem_tests.h"
#include "stringutils_tests.h"
#include "time_tests.h"
//#include "checksum_tests.h"
#include "thread_tests.h"
#include "lock_tests.h"
#include "filewriter_tests.h"
#include "swap_tests.h"
#include "math_tests.h"
#include "hash_tests.h"
#include "rand_tests.h"
#include "map_tests.h"
#include "tree_tests.h"
#include "hashmap_tests.h"
#include <stdio.h>

BFTEST_SUITE_FUNC({
	//BFTEST_SUITE_LAUNCH(checksum_tests);

	BFTEST_SUITE_LAUNCH(coreutils_tests);
	BFTEST_SUITE_LAUNCH(lock_tests);
	BFTEST_SUITE_LAUNCH(filesystem_tests);
	BFTEST_SUITE_LAUNCH(stringutils_tests);
	BFTEST_SUITE_LAUNCH(time_tests);
	BFTEST_SUITE_LAUNCH(thread_tests);
	BFTEST_SUITE_LAUNCH(filewriter_tests);
	BFTEST_SUITE_LAUNCH(swap_tests);
	BFTEST_SUITE_LAUNCH(rand_tests);
	BFTEST_SUITE_LAUNCH(hash_tests);
	BFTEST_SUITE_LAUNCH(math_tests);
	BFTEST_SUITE_LAUNCH(hashmap_tests);
	BFTEST_SUITE_LAUNCH(map_tests);
	BFTEST_SUITE_LAUNCH(tree_tests);

})

