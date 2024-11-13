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
#include <stdio.h>

int main() {
	TEST_SUITE_START;

	TEST_SUITE_LAUNCH(lock_tests);
	TEST_SUITE_LAUNCH(coreutils_tests);
	//TEST_SUITE_LAUNCH(checksum_tests);
	TEST_SUITE_LAUNCH(filesystem_tests);
	TEST_SUITE_LAUNCH(stringutils_tests);
	TEST_SUITE_LAUNCH(time_tests);
	TEST_SUITE_LAUNCH(thread_tests);
	TEST_SUITE_LAUNCH(filewriter_tests);
	TEST_SUITE_LAUNCH(swap_tests);
	
	TEST_SUITE_END;

	return 0;
}

