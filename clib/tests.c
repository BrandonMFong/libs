/**
 * author: Brando
 * date: 6/30/22
 */

#ifdef TESTING

#include "clib_tests.h"

int main() {
	int pass = 0, fail = 0;

	printf("\n---------------------------\n");
	printf("\nStarting C Library tests...\n\n");
	clib_tests(&pass, &fail);

	printf("\nPass: %d\n", pass);
	printf("Fail: %d\n", fail);
	printf("\n---------------------------\n\n");

	return 0;
}

#endif // TESTING

