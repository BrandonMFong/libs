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
	BFTEST_SUITE_START;

	BFTEST_SUITE_LAUNCH(coreutils_tests);
	BFTEST_SUITE_LAUNCH(lock_tests);
	//BFTEST_SUITE_LAUNCH(checksum_tests);
	BFTEST_SUITE_LAUNCH(filesystem_tests);
	BFTEST_SUITE_LAUNCH(stringutils_tests);
	BFTEST_SUITE_LAUNCH(time_tests);
	BFTEST_SUITE_LAUNCH(thread_tests);
	BFTEST_SUITE_LAUNCH(filewriter_tests);
	BFTEST_SUITE_LAUNCH(swap_tests);
	
	BFTEST_SUITE_END;

	return 0;
}

