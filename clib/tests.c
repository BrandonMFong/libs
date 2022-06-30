/**
 * author: Brando
 * date: 6/30/22
 */

#include "clib_tests.h"

int main() {
	int pass = 0, fail = 0;

	printf("\nStarting tests...\n");
	clib_tests(&pass, &fail);

	printf("\nPass: %d\n", pass);
	printf("Fail: %d\n", fail);
	return 0;
}
