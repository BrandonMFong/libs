/**
 * author: Brando
 * date: 7/31/22
 */

#ifndef LIST_TESTS_HPP
#define LIST_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include <list.hpp>

using namespace BF;

int test_Init() {
	int result = 0;

	List<int> * l = new List<int>;
	Delete(l);
	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_adding() {
	int result = 0;

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
			printf("Count is %d\n", l->count());
			result = 3;
		}
	}

	Delete(l);
	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_indexing() {
	int result = 0;
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
		printf("Count %d\n", l->count());
		printf("%f\n", l->objectAtIndex(0));
		printf("%f\n", l->objectAtIndex(1));
	}

	Delete(l);

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_inserting() {
	int result = 0;
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
		printf("Count %d\n", l->count());
		printf("0: %f\n", l->objectAtIndex(0));
		printf("1: %f\n", l->objectAtIndex(1));
		printf("2: %f\n", l->objectAtIndex(2));
	}

	Delete(l);

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_deletingAtIndex() {
	int result = 0;

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
		printf("Count %d\n", l->count());
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

	Delete(l);

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_deletingAllNodes() {
	int result = 0;

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

	PRINT_TEST_RESULTS(!result);
	return result;
}

void intDelete(int * i) {
	delete i;
}

int test_listMemoryHandling() {
	int result = 0;

	List<int *> * l = new List<int *>;
	l->setDeallocateCallback(intDelete);

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
			printf("count: %d\n", l->count());
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

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_traversing() {
	UNIT_TEST_START;

	int result = 0;
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

	int i = 0;
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

	UNIT_TEST_END(!result, result);
	return result;
}

int test_ListContains() {
	int result = 0;

	List<int> l;
	l.add(1);
	l.add(2);
	l.add(3);

	if (!l.contains(1)) {
		result = 1;
	} else if (l.contains(4)) {
		result = 2;
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_InitializingWithInitList() {
	int result = 0;
	
	List<int> l = {1,2,3,4};

	l = {5,6,7,8};

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_ListSortAscending() {
	UNIT_TEST_START;
	int result = 0;

	List<int> l;

	srand(time(NULL));

	int size = 2 << 20, max = size;
	for (int i = 0; i < size; i++) {
		int num = rand() % max;

		result = l.add(num);

		if (result) break;
	}

	if (!result)
		result = l.sort();

	if (!result) {
		List<int>::Node * n = l.first();
		int tmp = n->object();
		for (n = n->next(); n; n = n->next()) {
			if (tmp > n->object()) {
				printf("%d > %d\n", tmp, n->object());
				result = 2;
				break;
			}
			tmp = n->object();
		}
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_ListSortDescending() {
	UNIT_TEST_START;
	int result = 0;

	List<int> l;

	srand(time(NULL));

	int size = 2 << 20, max = 2 << 20;
	for (int i = 0; i < size; i++) {
		int num = rand() % max;

		result = l.add(num);

		if (result) break;
	}

	if (!result)
		result = l.sort(kListSortOptionsDescending);

	if (!result) {
		List<int>::Node * n = l.first();
		int tmp = n->object();
		for (n = n->next(); n; n = n->next()) {
			if (tmp < n->object()) {
				printf("%d < %d\n", tmp, n->object());
				result = 2;
				break;
			}
			tmp = n->object();
		}
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_InitializingFromRawArray() {
	int result = 0;

	const int size = 5;
	const char * strings[size] = {"one", "two", "three", "four", "five"};

	List<const char *> l;

	l.set(strings, size);

	if (l.count() != size) result = 1;

	for (int i = 0; (i < size) && !result; i++) {
		if (!l.contains(strings[i])) result = i + 10;
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_ListSortingStrings() {
	int result = 0;

	List<const char *> l = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten"};
	l.setCompareCallback(strcmp);

	result = l.sort();
	List<const char *>::Node * n = 0;
	const char * tmp = 0;
	if (!result) {
		tmp = l.first()->object();
		n = l.first()->next();
	}

	while (!result && n) {
		if (strcmp(tmp, n->object()) > 0) {
			result = 2;
			l.print();
		}

		n = n->next();
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_ListNullSwap() {
	int result = 0;
	List<int>::Node a, b;
	a.obj = 0;
	b.obj = 1;
	result = List<int>::swap(&a, 0);
	if (result) result = List<int>::swap(0, &b);
	if (result) result = List<int>::swap(0, 0);

	PRINT_TEST_RESULTS(result);
	return result == 0 ? -1 : 0;
}

int test_ListSwap() {
	int result = 0;
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
	
	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_shuffle() {
	UNIT_TEST_START;
	int result = 0;
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
	
	UNIT_TEST_END(!result, result);
	return result;
}

int test_ShuffleLargeDataSet() {
	UNIT_TEST_START;

	int result = 0;
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

	UNIT_TEST_END(!result, result);
	return result;
}

void list_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;
	LAUNCH_TEST(test_Init, p, f);
	LAUNCH_TEST(test_adding, p, f);
	LAUNCH_TEST(test_indexing, p, f);
	LAUNCH_TEST(test_inserting, p, f);
	LAUNCH_TEST(test_deletingAtIndex, p, f);
	LAUNCH_TEST(test_deletingAllNodes, p, f);
	LAUNCH_TEST(test_listMemoryHandling, p, f);
	LAUNCH_TEST(test_traversing, p, f);
	LAUNCH_TEST(test_ListContains, p, f);
	LAUNCH_TEST(test_InitializingWithInitList, p, f);
	LAUNCH_TEST(test_ListSortAscending, p, f);
	LAUNCH_TEST(test_ListSortDescending, p, f);
	LAUNCH_TEST(test_InitializingFromRawArray, p, f);
	LAUNCH_TEST(test_ListSortingStrings, p, f);
	LAUNCH_TEST(test_ListSwap, p, f);
	LAUNCH_TEST(test_shuffle, p, f);
	LAUNCH_TEST(test_ListNullSwap, p, f);
	LAUNCH_TEST(test_ShuffleLargeDataSet, p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // LIST_TESTS_HPP

