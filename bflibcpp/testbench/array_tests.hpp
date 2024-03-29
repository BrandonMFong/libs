/**
 * author: Brando
 * date: 6/30/22
 */

#ifndef ARRAY_TESTS_HPP
#define ARRAY_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include <array.hpp>
#include "cpplib_tests.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

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

	Array<int> arr({1, 2, 3, 4});

	if (!arr.contains(2)) {
		result = 1;
		printf("arr should contain 2\n");
	}

	Array<const char *> ch({"Hello", "world", "my", "name", "is", "lib"});
	ch.setComparator(strcmp);
	
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
	UNIT_TEST_START;

	int result = 0;
	int max = 2 << 20;
	while (!result && max) {
		Array<int> arr({1, 2, 3, 4});

		if (arr.indexForObject(3) != 2) {
			result = 1;
			printf("index should be 2 but is %ld\n", arr.indexForObject(3));
		}

		Array<const char *> ch({"Hello", "world", "my", "name", "is", "lib"});
		ch.setComparator(strcmp);
		
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
		if (ch.indexForObject(buf) != -1UL) {
			result = 1;
			printf("Index should be -1 but is %ld\n", ch.indexForObject(buf));
		}

		max--;
	}

	UNIT_TEST_END(!result, result);

	return result;

}

int test_Count() {
	int result = 0;

	Array<int> a({1, 2, 3, 4, 5});

	if (a.count() != 5) {
		result = 1;
		printf("Count %ld\n", a.count());
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
		printf("Count %ld\n", a.count());
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_addanddelete() {
	UNIT_TEST_START;
	int result = 0;

	int max = 2 << 8;
	while (!result && max) {
		int objcount = 2 << 16;
		Array<int> a;

		// add
		for (int i = 0; i < objcount; i++) {
			result = a.add(i);
			if (result) break;
		}

		// compare
		if (!result) {
			for (int i = 0; i < objcount; i++) {
				if (a[i] != i) {
					result = 2;
					break;
				}
			}
		}

		// delete elements from the end of the array
		while (!result && a.count()) {
			result = a.removeObjectAtIndex(a.count() - 1);
		}

		max--;
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_deletingObjectAtRandomIndex() {
	UNIT_TEST_START;
	int result = 0;

	int max = 2 << 10;
	while (!result && max) {
		int objcount = 2 << 12;
		Array<int, int> a;

		// add
		for (int i = 0; i < objcount; i++) {
			result = a.add(i);
			if (result) break;
		}

		// compare
		if (!result) {
			for (int i = 0; i < objcount; i++) {
				if (a[i] != i) {
					result = 2;
					break;
				}
			}
		}

		// delete at random index
		unsigned int randindex = 0;
		if (!result) {
			srand(time(0));
			randindex = ((unsigned int) rand() % objcount) - 1;
			result = a.removeObjectAtIndex(randindex);
		}

		if (!result) {
			// scan objects
			for (int i = 0; i < a.count(); i++) {
				int off = 0;
				if (i >= (int) randindex) {
					off = 1;
				}
				if (a[i] != (i + off)) {
					result = 3;
					break;
				}
			}
		}

		max--;
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_insertingAtRandomIndex() {
	UNIT_TEST_START;
	int result = 0;

	int max = 2 << 8;
	while (!result && max) {
		int objcount = 2 << 16;
		Array<int, int> a;

		// add
		for (int i = 0; i < objcount; i++) {
			result = a.add(i);
			if (result) break;
		}

		// compare
		if (!result) {
			for (int i = 0; i < objcount; i++) {
				if (a[i] != i) {
					result = 2;
					break;
				}
			}
		}

		// delete at random index
		unsigned int randindex = 0;
		int randnum = 0;
		if (!result) {
			srand(time(0));
			randindex = ((unsigned int) rand() % objcount) - 1;
			randnum = rand();
			result = a.insertObjectAtIndex(randnum, randindex);
		}

		if (!result) {
			// scan objects
			for (int i = 0; i < a.count(); i++) {
				if (i == (int) randindex) {
					if (a[i] != (int) randnum) {
						result = 4;
						break;
					}
				} else {
					int off = 0;
					if (i > (int) randindex) {
						off = 1;
					}

					if (a[i] != (i - off)) {
						result = 3;
						break;
					}
				}
			}
		}

		max--;
	}

	UNIT_TEST_END(!result, result);
	return result;
}

void TestArrayRelease(char * obj) {
	BFFree(obj);
}

int test_releasecallback() {
	UNIT_TEST_START;
	int result = 0;

	int max = 2 << 8;
	while (!result && max--) {
		Array<char *> a;

		a.setReleaseCallback(TestArrayRelease);

		srand(time(0));
		int asize = rand() % (2 << 16);
		for (int i = 0; i < asize; i++) {
			a.add(BFStringCopyString("word"));
		}
	}

	UNIT_TEST_END(!result, result);
	return result;
}

void array_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	LAUNCH_TEST(test_Initializer, p, f);
	LAUNCH_TEST(test_Contains, p, f);
	LAUNCH_TEST(test_ObjectAtIndex, p, f);
	LAUNCH_TEST(test_Count, p, f);
	LAUNCH_TEST(test_Setter, p, f);
	LAUNCH_TEST(test_indexForObject, p, f);
	LAUNCH_TEST(test_addanddelete, p, f);
	LAUNCH_TEST(test_deletingObjectAtRandomIndex, p, f);
	LAUNCH_TEST(test_insertingAtRandomIndex, p, f);
	LAUNCH_TEST(test_releasecallback, p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // ARRAY_TESTS_HPP

