/**
 * author: Brando
 * date: 6/30/22
 */

#include "Array.hpp"
#include "tests.hpp"
#include <stdio.h>
#include <stdlib.h>

/**
 * Just test that it can build and nothing crashes
 */
int test_Initializer() {
	Array<int> intArr;
	Array<char> charArr;
	Array<double> doubleArr;

	Array<int> * intArrPtr = new Array<int>();
	Array<double> * doubleArrPtr = new Array<double>();
	Array<char> * charArrPtr = new Array<char>();

	delete intArrPtr;
	delete doubleArrPtr;
	delete charArrPtr;

	PRINT_TEST_RESULTS(1);

	return 0;
}

int test_Contains() {
	int result = 0;

	PRINT_TEST_RESULTS(!result);

	return result;
}

void Array_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (!test_Initializer()) p++;
	else f++;
	
	if (!test_Contains()) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

