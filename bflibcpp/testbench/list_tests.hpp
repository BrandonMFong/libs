/**
 * author: Brando
 * date: 7/31/22
 */

#ifndef LIST_TESTS_HPP
#define LIST_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include <list.hpp>
#include "release.hpp"

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

BFTEST_UNIT_FUNC(test_Init, 2<<10,  {
	List<int> * l = new List<int>;
	BFRelease(l);

	List<int> list;
})

BFTEST_UNIT_FUNC(test_adding, 2<<10,  {
	List<int> * l = new List<int>;

	BF_ASSERT(l->add((int) 1) == 0, "Error adding 1");
	BF_ASSERT(l->add((int) 2) == 0, "Error adding 1");
	BF_ASSERT(l->count() == 2, "count is %ld", l->count());
	BFRelease(l);
})

BFTEST_UNIT_FUNC(test_indexing, 2<<10, {
	List<double> * l = new List<double>;

	BF_ASSERT(l != NULL);
	BF_ASSERT(l->add(1.0) == 0);
	BF_ASSERT(l->add(1.2) == 0);
	BF_ASSERT(l->add(1.3) == 0);
	BF_ASSERT(l->objectAtIndex(0) == 1.0);
	BF_ASSERT(l->objectAtIndex(1) == 1.2);

	BFRelease(l);
})

BFTEST_UNIT_FUNC(test_inserting, 2<<10, {
	List<double> * l = new List<double>;

	BF_ASSERT(l != NULL);
	BF_ASSERT(!l->add(1.0));
	BF_ASSERT(!l->add(1.3));
	BF_ASSERT(l->objectAtIndex(0) == 1.0);
	BF_ASSERT(l->objectAtIndex(1) == 1.3);
	BF_ASSERT(!l->insertObjectAtIndex(1.2, 1));
	BF_ASSERT(l->objectAtIndex(1) == 1.2);
	BF_ASSERT(l->objectAtIndex(2) == 1.3);

	BFRelease(l);
})

BFTEST_UNIT_FUNC(test_deletingAtIndex, 2<<10, {
	List<long> * l = new List<long>;

	BF_ASSERT(!l->add(1));
	BF_ASSERT(!l->add(2));
	BF_ASSERT(!l->add(3));
	BF_ASSERT(!l->deleteObjectAtIndex(1));
	BF_ASSERT(l->objectAtIndex(1) == 3);
	BF_ASSERT(l->count() == 2);
	BF_ASSERT(l->objectAtIndex(0) == 1);
	BF_ASSERT(!l->deleteObjectAtIndex(0));
	BF_ASSERT(!l->deleteObjectAtIndex(0));

	BFRelease(l);
})

BFTEST_UNIT_FUNC(test_deletingAllNodes, 2<<10, {
	List<int> * l = new List<int>;

	int listsize = 2<<8;
	for (int i = 0; i < listsize; i++) {
		BF_ASSERT(!l->add(i));
	}
	BF_ASSERT(l->count() == listsize);
	
	l->deleteAll();
	BF_ASSERT(l->count() == 0);

	BFRelease(l);
})

void intDelete(int * i) {
	delete i;
}

BFTEST_UNIT_FUNC(test_listMemoryHandling, 2<<10, {
	if (BFTEST_UNIT_FUNC_ITR == 0) {
		BFRandInit(time(0));
	}

	List<int *> * l = new List<int *>;
	BF_ASSERT(l);
	l->setReleaseCallback(intDelete);

	int listsize = 2<<8;
	for (int i = 0; i < listsize; i++) {
		int * num = new int;
		*num = i;
		BF_ASSERT(!l->add(num));
	}

	BF_ASSERT(l->count() == listsize, "count: %ld\n", l->count());
	
	BF_ASSERT(!l->deleteObjectAtIndex(0));
	BF_ASSERT(l->count() == (listsize - 1), "count: %ld\n", l->count());

	int index = BFRand() % l->count();
	int * num = l->objectAtIndex(index);
	BF_ASSERT(num != NULL, "num is null");
	BF_ASSERT(*num == index+1, "list[%d]=%d", index, *num);

	l->deleteAll();
	BFRelease(l);
})

BFTEST_UNIT_FUNC(test_traversing, 2<<10, {
	if (BFTEST_UNIT_FUNC_ITR == 0) {
		BFRandInit(time(0));
	}
	int max = 2 << 31; // max nodes
	int minValue = 10;
	
	List<int> t;
	while (max--) {
		int val = (BFRand() % (2 << 16)) + minValue;
		BF_ASSERT(!t.add(val));
	}

	size_t i = 0;
	List<int>::Node * node = t.first();
	while ((i < t.count()) && node) {
		node = node->next();
		i++;
	}

	BF_ASSERT(i == t.count(), "Traversing did not go through entire list");
})

BFTEST_UNIT_FUNC(test_ListContains, 2<<10, {
	if (BFTEST_UNIT_FUNC_ITR == 0) {
		BFRandInit(time(0));
	}
	List<int> l;
	int listsize = 2 << 8;
	for (int i = 0; i < listsize; i++) {
		l.add(i);
	}

	int val = BFMathAbs(BFRand()) % listsize;
	BF_ASSERT(l.contains(val));

	val += listsize;
	BF_ASSERT(!l.contains(val));
})

BFTEST_UNIT_FUNC(test_InitializingWithInitList, 2 << 10, {
	List<int> l = {1,2,3,4};
	BF_ASSERT(l.contains(3));
	BF_ASSERT(!l.contains(10));
	
	l = {5,6,7,8};
	BF_ASSERT(l.contains(5));
	BF_ASSERT(!l.contains(10));
})

BFTEST_UNIT_FUNC(test_InitializingFromRawArray, 2<<10, {
	const size_t size = 5;
	const char * strings[size] = {"one", "two", "three", "four", "five"};

	List<const char *> l;

	l.set(strings, size);

	BF_ASSERT(l.count() == size);

	for (size_t i = 0; (i < size) && !result; i++) {
		BF_ASSERT(l.contains(strings[i]), "list does not contain %s", strings[i]);
	}
})

BFTEST_UNIT_FUNC(test_ListNullSwap, 2<<10,  {
	List<int>::Node a, b;
	a.obj = 0;
	b.obj = 1;
	BF_ASSERT(List<int>::swap(&a, 0) != 0);
	BF_ASSERT(List<int>::swap(0, &b) != 0);
	BF_ASSERT(List<int>::swap(0, 0) != 0);
})

BFTEST_UNIT_FUNC(test_ListSwap, 2<<10, {
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

	BF_ASSERT(a->obj == 1);
	BF_ASSERT(a->prev() == al);
	BF_ASSERT(a->next() == ar);
	BF_ASSERT(al->next() == a);
	BF_ASSERT(ar->prev() == a);
	BF_ASSERT(b->obj == 2);
	BF_ASSERT(b->prev() == bl);
	BF_ASSERT(b->next() == br);
	BF_ASSERT(bl->next() == b);
	BF_ASSERT(br->prev() == b);

	BF_ASSERT(!List<int>::swap(a, b));
	BF_ASSERT(a->obj == 2);
	BF_ASSERT(a->prev() == al);
	BF_ASSERT(a->next() == ar);
	BF_ASSERT(al->next() == a);
	BF_ASSERT(ar->prev() == a);
	BF_ASSERT(b->obj == 1);
	BF_ASSERT(b->prev() == bl);
	BF_ASSERT(b->next() == br);
	BF_ASSERT(bl->next() == b);
	BF_ASSERT(br->prev() == b);
	
	BFRelease(a);
	BFRelease(al);
	BFRelease(ar);
	BFRelease(b);
	BFRelease(bl);
	BFRelease(br);
})

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

BFTEST_UNIT_FUNC(test_rangeBasedLooping, 2<<10, {
	if (BFTEST_UNIT_FUNC_ITR == 0) {
		BFRandInit(time(0));
	}
	List<int> list;
	int arrsize = BFMathAbs(BFRand()) % 2 << 15;
	int * arr = (int *) malloc(sizeof(int) * arrsize);
	for (int i = 0; i < arrsize; i++) {
		arr[i] = rand();
		BF_ASSERT(!list.add(arr[i]));
	}

	int i = 0;
	for (int a : list) {
		BF_ASSERT(a == arr[i], "%d != %d\n", a, arr[i]);
		i++;
	}

	BF_ASSERT(i == arrsize, "we did not go through the entire list: %d != %d\n", i, arrsize);

	BFFree(arr);
})

BFTEST_COVERAGE_FUNC(list_tests, {
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
})

#endif // LIST_TESTS_HPP

