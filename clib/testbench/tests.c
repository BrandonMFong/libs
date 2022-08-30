/**
 * author: Brando
 * date: 6/30/22
 */

#include "coreutils_tests.h"
#include "filesystem_tests.h"
#include <stdio.h>

int main() {
	int pass = 0, fail = 0;

	printf("\n---------------------------\n");
	printf("\nStarting Core Utilities tests...\n\n");
	coreutils_tests(&pass, &fail);

	printf("\nPass: %d\n", pass);
	printf("Fail: %d\n", fail);

	printf("\n---------------------------\n");
	printf("\nStarting File system Utilities tests...\n\n");
	filesystem_tests(&pass, &fail);

	printf("\nPass: %d\n", pass);
	printf("Fail: %d\n", fail);
	printf("\n---------------------------\n\n");

	return 0;
}

