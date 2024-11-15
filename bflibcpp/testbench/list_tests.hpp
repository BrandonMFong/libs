/**
 * author: Brando
 * date: 7/31/22
 */

#ifndef LIST_TESTS_HPP
#define LIST_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include <list.hpp>
#include "release.hpp"

using namespace BF;

//int test_Init() {
BFTEST_UNIT_FUNC(test_Init, 1,  {
	List<int> * l = new List<int>;
	BFRelease(l);
})

//int test_adding() {
BFTEST_UNIT_FUNC(test_adding, 1,  {
	List<int> * l = new List<int>;

	if ((result = l->add((int) 1)) != 0) {
		printf("Error adding 1 (%d)\n", result);
		result = 1;
	}

	if (!result) {
		if ((result = l->add((int) 2)) != 0) {
			printf("Error adding 2 (%d)\n", result);
			result = 1;
		}
	}

	if (!result) {
		if (l->count() != 2) {
			printf("Count is %ld\n", l->count());
			result = 3;
		}
	}

	BFRelease(l);
})

//int test_indexing() {
BFTEST_UNIT_FUNC(test_indexing, 1,  {
	List<double> * l = new List<double>;

	if (l == 0) {
		result = 1;
	} else if (l->add(1.0)) {
		result = 2;
	} else if (l->add(1.2)) {
		result = 3;
	} else if (l->add(1.3)) {
		result = 4;
	} else if (l->objectAtIndex(0) != 1.0) {
		result = 5;
	} else if (l->objectAtIndex(1) != 1.2) {
		result = 6;
	}

	if (result) {
		printf("Error %d\n", result);
		printf("Count %ld\n", l->count());
		printf("%f\n", l->objectAtIndex(0));
		printf("%f\n", l->objectAtIndex(1));
	}

	BFRelease(l);
})

//int test_inserting() {
BFTEST_UNIT_FUNC(test_inserting, 1,  {
	List<double> * l = new List<double>;

	if (l == 0) {
		result = 1;
	} else if (l->add(1.0)) {
		result = 2;
	} else if (l->add(1.3)) {
		result = 3;
	} else if (l->objectAtIndex(0) != 1.0) {
		result = 5;
	} else if (l->objectAtIndex(1) != 1.3) {
		result = 6;
	} else if (l->insertObjectAtIndex(1.2, 1)) {
		result = 7;
	} else if (l->objectAtIndex(1) != 1.2) {
		result = 8;
	} else if (l->objectAtIndex(2) != 1.3) {
		result = 9;
	}

	if (result) {
		printf("Error %d\n", result);
		printf("Count %ld\n", l->count());
		printf("0: %f\n", l->objectAtIndex(0));
		printf("1: %f\n", l->objectAtIndex(1));
		printf("2: %f\n", l->objectAtIndex(2));
	}

	BFRelease(l);
})

//int test_deletingAtIndex() {
BFTEST_UNIT_FUNC(test_deletingAtIndex, 1,  {
	List<long> * l = new List<long>;

	if (l->add(1)) {
		result = 1;
	} else if (l->add(2)) {
		result = 2;
	} else if (l->add(3)) {
		result = 3;
	} else if (l->deleteObjectAtIndex(1)) {
		result = 4;
	} else if (l->objectAtIndex(1) != 3) {
		result = 5;
		printf("1: %ld\n", l->objectAtIndex(1));
	} else if (l->count() != 2) {
		result = 6;
		printf("Count %ld\n", l->count());
	} else if (l->objectAtIndex(0) != 1) {
		result = 7;
	} else if (l->deleteObjectAtIndex(0)) {
		result = 8;
	} else if (l->deleteObjectAtIndex(0)) {
		result = 9;
	}

	if (result) {
		printf("Error %d\n", result);
	}

	BFRelease(l);
})

//int test_deletingAllNodes() {
BFTEST_UNIT_FUNC(test_deletingAllNodes, 1,  {
	List<char> * l = new List<char>;

	if (l->add('a')) {
		result = 5;
	} else if (l->add('b')) {

		result = 1;
	} else if (l->add('c')) {
		result = 2;
	} else if (l->add('d')) {
		result = 3;
	} else if (l->count() != 4) {
		result = 4;
	} else {
		l->deleteAll();
		result = l->count() ? 7 : 0;
	}

	if (result) {
		printf("Error %d\n", result);
	}
})

void intDelete(int * i) {
	delete i;
}

//int test_listMemoryHandling() {
BFTEST_UNIT_FUNC(test_listMemoryHandling, 1,  {
	List<int *> * l = new List<int *>;
	l->setReleaseCallback(intDelete);

	int * i = 0;

	i = new int;
	*i = 1;
	result = l->add(i);

	if (result == 0) {
		i = new int;
		*i = 2;
		result = l->add(i);
	}

	if (result == 0) {
		i = new int;
		*i = 3;
		result = l->add(i);
	}

	if (result == 0) {
		if (l->count() != 3) {
			printf("count: %ld\n", l->count());
			result = 1;
		}
	}

	if (result == 0) {
		result = l->deleteObjectAtIndex(1);
	}

	if (result == 0) {
		i = l->objectAtIndex(1);
		if (*i != 3) {
			printf("1: %d\n", *i);
			result = 1;
		}
	}

	l->deleteAll();
})

//int test_traversing() {
BFTEST_UNIT_FUNC(test_traversing, 1,  {
	int max = 2 << 31; // max nodes
	int minValue = 10;
	
	List<int> t;

	while (max) {
		int val = (rand() % (2 << 16)) + minValue;

		result = t.add(val);
		if (result) {
			printf("error: %d\n", result);
			break;
		}

		max--;
	}

	size_t i = 0;
	List<int>::Node * node = t.first();
	while (!result && (i < t.count()) && node) {
		node = node->next();
		i++;
	}

	if (result == 0) {
		if (i != t.count()) {
			printf("Traversing did not go through entire list\n");
			result = 1;
		}
	}
})

//int test_ListContains() {
BFTEST_UNIT_FUNC(test_ListContains, 1,  {
	List<int> l;
	l.add(1);
	l.add(2);
	l.add(3);

	if (!l.contains(1)) {
		result = 1;
	} else if (l.contains(4)) {
		result = 2;
	}
})

//int test_InitializingWithInitList() {
BFTEST_UNIT_FUNC(test_InitializingWithInitList, 1,  {
	List<int> l = {1,2,3,4};

	l = {5,6,7,8};
})

//int test_InitializingFromRawArray() {
BFTEST_UNIT_FUNC(test_InitializingFromRawArray, 1,  {
	const size_t size = 5;
	const char * strings[size] = {"one", "two", "three", "four", "five"};

	List<const char *> l;

	l.set(strings, size);

	if (l.count() != size) result = 1;

	for (size_t i = 0; (i < size) && !result; i++) {
		if (!l.contains(strings[i])) result = i + 10;
	}
})

//int test_ListNullSwap() {
BFTEST_UNIT_FUNC(test_ListNullSwap, 1,  {
	List<int>::Node a, b;
	a.obj = 0;
	b.obj = 1;
	result = List<int>::swap(&a, 0);
	if (result) result = List<int>::swap(0, &b);
	if (result) result = List<int>::swap(0, 0);
})

BFTEST_UNIT_FUNC(test_ListSwap, 1,  {
	List<int>::Node * a = new List<int>::Node;
	a->obj = 1;
	List<int>::Node * al = new List<int>::Node;
	List<int>::Node * ar = new List<int>::Node;
	a->left = al;
	a->right = ar;
	al->right = a;
	ar->left = a;

	List<int>::Node * b = new List<int>::Node;
	b->obj = 2;
	List<int>::Node * bl = new List<int>::Node;
	List<int>::Node * br = new List<int>::Node;
	b->left = bl;
	b->right = br;
	bl->right = b;
	br->left = b;

	if (a->obj != 1) result = 1;
	else if (a->prev() != al) result = 1;
	else if (a->next() != ar) result = 1;
	else if (al->next() != a) result = 1;
	else if (ar->prev() != a) result = 1;
	else if (b->obj != 2) result = 1;
	else if (b->prev() != bl) result = 1;
	else if (b->next() != br) result = 1;
	else if (bl->next() != b) result = 1;
	else if (br->prev() != b) result = 1;

	if (result == 0) result = List<int>::swap(a, b);
	if (result == 0) {
		if (a->obj != 2) result = 2;
		else if (a->prev() != al) result = 2;
		else if (a->next() != ar) result = 2;
		else if (al->next() != a) result = 2;
		else if (ar->prev() != a) result = 2;
		else if (b->obj != 1) result = 2;
		else if (b->prev() != bl) result = 2;
		else if (b->next() != br) result = 2;
		else if (bl->next() != b) result = 2;
		else if (br->prev() != b) result = 2;
	}
	BFRelease(a);
	BFRelease(al);
	BFRelease(ar);
	BFRelease(b);
	BFRelease(bl);
	BFRelease(br);
})

//int test_shuffle() {
BFTEST_UNIT_FUNC(test_shuffle, 1,  {
	const int size = 2 << 14;
	int array[size];
	for (int i = 0; i < size; i++) {
		array[i] = i;
	}
	List<int> list;
	list.set(array, size);
	result = list.shuffle();

	if (result == 0) {
		int score = 0;
		int i = 0;
		for (List<int>::Node * n = list.first();
			n; n = n->next()) {
			if ((i < size) && (n->object() == array[i])) score++;
			i++;
		}

		// If I record that all of the elements in both 
		// arrays match at every index then we didn't 
		// shuffle correctly
		if (score == size) {
			result = 5;
		}
	}
})

//int test_ShuffleLargeDataSet() {
BFTEST_UNIT_FUNC(test_ShuffleLargeDataSet, 1,  {
	srand(time(0));
	const int size = 2 << 14;
	int array[size];
	for (int i = 0; i < size; i++) {
		array[i] = rand();
	}
	List<int> list;
	list.set(array, size);
	result = list.shuffle();

	if (result == 0) {
		int score = 0;
		int i = 0;
		for (List<int>::Node * n = list.first();
			n; n = n->next()) {
			if ((i < size) && (n->object() == array[i])) score++;
			i++;
		}

		// If I record that all of the elements in both 
		// arrays match at every index then we didn't 
		// shuffle correctly
		if (score == size) {
			result = 5;
		}
	}
})

bool TestPluckingObjectReleaseWasCalled = false;
void TestPluckingObjectRelease(int * i) {
	BFFree(i);
	TestPluckingObjectReleaseWasCalled = true;
}

//int test_pluckingObject() {
BFTEST_UNIT_FUNC(test_pluckingObject, 2,  {
	srand(time(0));

	// make array and list
	size_t size = 10;
	int * arr[size];
	List<int *> list;
	list.setReleaseCallback(TestPluckingObjectRelease);
	for (size_t i = 0; i < size; i++) {
		arr[i] = (int *) malloc(sizeof(int));
		*arr[i] = rand();
		list.add(arr[i]);
	}

	// find a random position to delete from
	int del = rand() % size;
	int val = *arr[del];
	list.pluckObject(arr[del]);

	// test size
	if (list.count() + 1 != size) {
		result = max;
	}

	// make sure value hasn't been altered
	if (!result) {
		if (*arr[del] != val) {
			result = max;
		}
	}

	// make sure we can still play around with value
	if (!result) {
		for (size_t i = 0; i < size; i++) {
			int t = *arr[i];
			t++;
		}
		
		// see if the release func was called
		if (TestPluckingObjectReleaseWasCalled) {
			result = max;
		}
	}

	list.setReleaseCallback(0);
	for (size_t i = 0; i < size; i++) { BFFree(arr[i]); }
})

//int test_rangeBasedLooping() {
BFTEST_UNIT_FUNC(test_rangeBasedLooping, 2<<10,  {
	List<int> list;
	srand(time(0));
	int arrsize = rand() % 2 << 15;
	int * arr = (int *) malloc(sizeof(int) * arrsize);
	for (int i = 0; i < arrsize; i++) {
		arr[i] = rand();
		list.add(arr[i]);
	}

	int i = 0;
	for (int a : list) {
		if (a != arr[i]) {
			printf("%d != %d\n", a, arr[i]);
			result = max;
			break;
		}
		i++;
	}

	if (result) continue;

	if (i != arrsize) {
		printf("we did not go through the entire list: %d != %d\n", i, arrsize);
		result = max;
		continue;
	}

	BFFree(arr);
})

BFTEST_COVERAGE_FUNC(list_tests) {
	BFTEST_COVERAGE_START;

	BFTEST_LAUNCH(test_Init);
	BFTEST_LAUNCH(test_adding);
	BFTEST_LAUNCH(test_indexing);
	BFTEST_LAUNCH(test_inserting);
	BFTEST_LAUNCH(test_deletingAtIndex);
	BFTEST_LAUNCH(test_deletingAllNodes);
	BFTEST_LAUNCH(test_listMemoryHandling);
	BFTEST_LAUNCH(test_traversing);
	BFTEST_LAUNCH(test_ListContains);
	BFTEST_LAUNCH(test_InitializingWithInitList);
	BFTEST_LAUNCH(test_InitializingFromRawArray);
	BFTEST_LAUNCH(test_ListSwap);
	BFTEST_LAUNCH(test_shuffle);
	BFTEST_LAUNCH(test_ListNullSwap);
	BFTEST_LAUNCH(test_ShuffleLargeDataSet);
	BFTEST_LAUNCH(test_pluckingObject);
	BFTEST_LAUNCH(test_rangeBasedLooping);

	BFTEST_COVERAGE_END;
}

#endif // LIST_TESTS_HPP

