/**
 * author: Brando
 * date: 7/31/22
 */

#ifndef LIST_TESTS_HPP
#define LIST_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include "../list.hpp"

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
	l->setObjectMemoryHandler(intDelete);

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
	int result = 0;
	int max = 50; // max nodes
	int minValue = 10;
	
	List<int> t;

	while (max) {
		int val = (rand() % 100) + minValue;

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

	PRINT_TEST_RESULTS(!result);
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
	int result = 0;

	List<int> l;

	srand(time(NULL));

	int size = 1000, max = 100;
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

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_ListSortDescending() {
	int result = 0;

	List<int> l;

	srand(time(NULL));

	int size = 1000, max = 100;
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

	PRINT_TEST_RESULTS(!result);
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


void list_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (!test_Init()) p++;
	else f++;

	if (!test_adding()) p++;
	else f++;

	if (!test_indexing()) p++;
	else f++;

	if (!test_inserting()) p++;
	else f++;

	if (!test_deletingAtIndex()) p++;
	else f++;

	if (!test_deletingAllNodes()) p++;
	else f++;

	if (!test_listMemoryHandling()) p++;
	else f++;

	if (!test_traversing()) p++;
	else f++;

	if (!test_ListContains()) p++;
	else f++;

	if (!test_InitializingWithInitList()) p++;
	else f++;

	if (!test_ListSortAscending()) p++;
	else f++;

	if (!test_ListSortDescending()) p++;
	else f++;

	if (!test_InitializingFromRawArray()) p++;
	else f++;

	if (!test_ListSortingStrings()) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // LIST_TESTS_HPP

