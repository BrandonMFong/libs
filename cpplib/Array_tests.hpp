/**
 * author: Brando
 * date: 6/30/22
 */

#ifdef TESTING
#ifndef ARRAY_TESTS_HPP
#define ARRAY_TESTS_HPP

#include "Array.hpp"
#include "tests.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

ArrayComparisonResult StringCompare(const char * a, const char * b) {
	int result = strcmp((char *) a, (char *) b);

	if (result < 0) {
		return kArrayComparisonResultLessThan;
	} else if (result > 0) {
		return kArrayComparisonResultGreaterThan;
	} else if (result == 0) {
		return kArrayComparisonResultEquals;
	} else {
		return kArrayComparisonResultUnknown;
	}
}

int test_Contains() {
	int result = 0;

	Array<int> arr({1, 2, 3, 4});

	if (!arr.contains(2)) {
		result = 1;
		printf("arr should contain 2\n");
	}

	Array<const char *> ch({"Hello", "world", "my", "name", "is", "lib"});
	ch.setComparator(StringCompare);
	
	char buf[100];
	strcpy(buf, "world");
	if (!ch.contains(buf)) {
		result = 1;
		printf("ch should contain 'world'\n");
	}
	
	strcpy(buf, "Hello");
	if (!ch.contains(buf)) {
		result = 1;
		printf("ch should contain 'world'\n");
	}
	
	strcpy(buf, "hello");
	if (ch.contains(buf)) {
		result = 1;
		printf("ch should contain 'world'\n");
	}

	PRINT_TEST_RESULTS(!result);

	return result;
}

int test_ObjectAtIndex() {
	int result = 0;

	Array<double> d({1.1, 2.2, 3.3, 4.4, 5.5});

	double a = d.objectAtIndex(2);

	if (a != 3.3) {
		result = 1;
		printf("%f != 3.3\n", a);
	}

	a = d[3];

	if (a != 4.4) {
		result = 1;
		printf("%f != 4.4\n", a);
	}

	PRINT_TEST_RESULTS(!result);

	return result;
}

int test_indexForObject() {
	int result = 0;

	Array<int> arr({1, 2, 3, 4});

	if (arr.indexForObject(3) != 2) {
		result = 1;
		printf("index should be 2 but is %ld\n", arr.indexForObject(3));
	}

	Array<const char *> ch({"Hello", "world", "my", "name", "is", "lib"});
	ch.setComparator(StringCompare);
	
	char buf[100];
	strcpy(buf, "world");
	if (ch.indexForObject(buf) != 1) {
		result = 1;
		printf("index should be 1 but is %ld\n", ch.indexForObject(buf));
	}
	
	strcpy(buf, "Hello");
	if (ch.indexForObject(buf) != 0) {
		result = 1;
		printf("index should be 0 but is %ld\n", ch.indexForObject(buf));
	}
	
	strcpy(buf, "hello");
	if (ch.indexForObject(buf) != -1) {
		result = 1;
		printf("Index should be -1 but is %ld\n", ch.indexForObject(buf));
	}

	PRINT_TEST_RESULTS(!result);

	return result;

}

int test_Count() {
	int result = 0;

	Array<int> a({1, 2, 3, 4, 5});

	if (a.count() != 5) {
		result = 1;
		printf("Count %d\n", a.count());
	}

	PRINT_TEST_RESULTS(!result);

	return result;
}

int test_Setter() {
	int result = 0;

	Array<int> a;

	a = {1, 2, 3, 4, 5};

	if (a.count() != 5) {
		result = 1;
		printf("Count %d\n", a.count());
	}

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

	if (!test_ObjectAtIndex()) p++;
	else f++;

	if (!test_Count()) p++;
	else f++;
	
	if (!test_Setter()) p++;
	else f++;

	if (!test_indexForObject()) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // TESTING
#endif // ARRAY_TESTS_HPP

