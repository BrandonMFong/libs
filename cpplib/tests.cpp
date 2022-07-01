/**
 * author: Brando
 * date: 6/30/22
 */

#ifdef CPPLIB_TESTING
#ifdef TESTING

#include "Array_tests.hpp"

int main() {
	int pass = 0, fail = 0;

	printf("\n---------------------------\n");
	printf("\nStarting C++ Library tests...\n\n");
	Array_tests(&pass, &fail);

	printf("\nPass: %d\n", pass);
	printf("Fail: %d\n", fail);
	printf("\n---------------------------\n\n");

	return 0;
}

#endif // TESTING
#endif // CPPLIB_TESTING

