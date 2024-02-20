/**
 * author: Brando
 * date: 6/30/22
 */

#include "coreutils_tests.h"
#include "filesystem_tests.h"
#include "stringutils_tests.h"
#include "time_tests.h"
#include "checksum_tests.h"
#include "thread_tests.h"
#include "lock_tests.h"
#include "filewriter_tests.h"
#include <stdio.h>

int main() {
	TEST_SUITE_START;

	printf("\n---------------------------\n");
	LAUNCH_TEST_SET(coreutils_tests);
	LAUNCH_TEST_SET(checksum_tests);
	LAUNCH_TEST_SET(filesystem_tests);
	LAUNCH_TEST_SET(stringutils_tests);
	LAUNCH_TEST_SET(time_tests);
	LAUNCH_TEST_SET(lock_tests);
	LAUNCH_TEST_SET(thread_tests);
	LAUNCH_TEST_SET(filewriter_tests);
	
	TEST_SUITE_END;

	return 0;
}

