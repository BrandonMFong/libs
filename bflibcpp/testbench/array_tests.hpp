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
#include "string.hpp"

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

/**
 * Just test that it can build and nothing crashes
 */
BFTEST_UNIT_FUNC(test_Initializer, 1, {
	Array<int> intArr;
	Array<char> charArr;
	Array<double> doubleArr;

	Array<int> * intArrPtr = new Array<int>();
	Array<double> * doubleArrPtr = new Array<double>();
	Array<char> * charArrPtr = new Array<char>();

	delete intArrPtr;
	delete doubleArrPtr;
	delete charArrPtr;
})

BFTEST_UNIT_FUNC(test_Contains, 1,  {
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
})

//int test_ObjectAtIndex() {
BFTEST_UNIT_FUNC(test_ObjectAtIndex, 1,  {
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
})

//int test_indexForObject() {
BFTEST_UNIT_FUNC(test_indexForObject, 2<<10,  {
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
})

//int test_Count() {
BFTEST_UNIT_FUNC(test_Count, 1,  {
	Array<int> a({1, 2, 3, 4, 5});

	if (a.count() != 5) {
		result = 1;
		printf("Count %ld\n", a.count());
	}
})

//int test_Setter() {
BFTEST_UNIT_FUNC(test_Setter, 1,  {
	Array<int> a;

	a = {1, 2, 3, 4, 5};

	if (a.count() != 5) {
		result = 1;
		printf("Count %ld\n", a.count());
	}

	if (!result) {
		a[0] = 6;
		if (a[0] != 6) {
			result = 2;
		}
	}
})

BFTEST_UNIT_FUNC(test_addanddelete, 2<<8,  {
	int objcount = 2 << 9;
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
})

BFTEST_UNIT_FUNC(test_deletingObjectAtRandomIndex, 2<<10,  {
	int objcount = 2<<10;
	Array<int, int> a;

	// add
	for (int i = 0; i < objcount; i++) {
		BF_ASSERT(!a.add(i));
	}

	// compare
	for (int i = 0; i < objcount; i++) {
		BF_ASSERT(a[i] == i);
	}

	// delete at random index
	unsigned int randindex = 0;
	srand(time(0));
	randindex = ((unsigned int) rand() % objcount) - 1;
	BF_ASSERT(!a.removeObjectAtIndex(randindex));

	// scan objects
	for (int i = 0; i < a.count(); i++) {
		int off = 0;
		if (i >= (int) randindex) {
			off = 1;
		}
		
		BF_ASSERT(a[i] == (i + off));
	}
})

BFTEST_UNIT_FUNC(test_insertingAtRandomIndex, 2<<8,  {
	int objcount = 2<<10;
	Array<int, int> a;

	// add
	for (int i = 0; i < objcount; i++) {
		BF_ASSERT(!a.add(i));
	}

	// compare
	for (int i = 0; i < objcount; i++) {
		BF_ASSERT(a[i] == i);
	}

	// delete at random index
	unsigned int randindex = 0;
	int randnum = 0;
	srand(time(0));
	randindex = ((unsigned int) rand() % objcount) - 1;
	randnum = rand();
	BF_ASSERT(!a.insertObjectAtIndex(randnum, randindex));

	// scan objects
	for (int i = 0; i < a.count(); i++) {
		if (i == (int) randindex) {
			BF_ASSERT(a[i] == (int) randnum);
		} else {
			int off = 0;
			if (i > (int) randindex) {
				off = 1;
			}

			BF_ASSERT(a[i] == (i - off));
		}
	}
})

void TestArrayRelease(char * obj) {
	BFFree(obj);
}

BFTEST_UNIT_FUNC(test_releasecallback, 2<<8,  {
	Array<char *> a;

	a.setReleaseCallback(TestArrayRelease);

	srand(time(0));
	int asize = rand() % (2 << 8);
	for (int i = 0; i < asize; i++) {
		BF_ASSERT(!a.add(BFStringCopyString("word")));
	}
})

int TestArrayCompareCStrings(char * a, char * b) {
	return strcmp(a, b);
}

BFTEST_UNIT_FUNC(test_arrayOfCStrings, 2<<8, {
	Array<char *> b;
	b.setComparator(TestArrayCompareCStrings);
	b.setReleaseCallback(TestArrayRelease);
	b.add(BFStringCopyString("hello"));
	b.add(BFStringCopyString("world"));

	char word[32];
	strcpy(word, "hello");
	BF_ASSERT(b.contains(word));
	strcpy(word, "world");
	BF_ASSERT(b.contains(word));
})

BFTEST_UNIT_FUNC(test_arrayOfBFStrings, 2<<8, {
	Array<BF::String> a({"hello", "world"});
	Array<BF::String> b;
	b.add("hello");
	b.add("world");
})

BFTEST_UNIT_FUNC(test_appendingArrays, 2 << 10, {
	int max_size = 2 << 10;
	Array<int> c;
	Array<int> d;
	for (int i = 0; i < max_size/2; i++) {
		c.add(i);
		d.add(i + max_size/2);
	}
	
	c.append(d);
	for (int j = 0; j < max_size; j++) {
		BF_ASSERT(c[j] == j, "%d != %d", c[j], j);
	}
})

BFTEST_COVERAGE_FUNC(array_tests, {
	BFTEST_LAUNCH(test_Initializer);
	BFTEST_LAUNCH(test_Contains);
	BFTEST_LAUNCH(test_ObjectAtIndex);
	BFTEST_LAUNCH(test_Count);
	BFTEST_LAUNCH(test_Setter);
	BFTEST_LAUNCH(test_indexForObject);
	BFTEST_LAUNCH(test_deletingObjectAtRandomIndex);
	BFTEST_LAUNCH(test_insertingAtRandomIndex);
	BFTEST_LAUNCH(test_releasecallback);
	BFTEST_LAUNCH(test_addanddelete);
	//BFTEST_LAUNCH(test_arrayOfBFStrings); // FIXME: this should work if we change how array's allocate memory
	BFTEST_LAUNCH(test_arrayOfCStrings);
	BFTEST_LAUNCH(test_appendingArrays);

})

#endif // ARRAY_TESTS_HPP

