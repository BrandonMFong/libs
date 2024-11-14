/**
 * author: Brando
 * date: 11/13/24
 */

#include "suite1_tests.h"

int main() {
	BFTEST_SUITE_START;

	BFTEST_SUITE_LAUNCH(suite1_tests);
	
	BFTEST_SUITE_END;

	return 0;
}

