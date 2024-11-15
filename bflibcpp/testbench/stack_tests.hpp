/**
 * author: Brando
 * date: 11/3/22
 */

#ifndef STACK_TESTS_HPP
#define STACK_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include <stack.hpp>

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

//int test_StackInit() {
BFTEST_UNIT_FUNC(test_StackInit, 1,  {
	Stack<int> stack;
})

//int test_StackPush() {
BFTEST_UNIT_FUNC(test_StackPush, 1,  {
	Stack<int> stack;

	result = stack.push(1);

	if (!result)
		result = stack.push(2);

	if (!result)
		result = stack.push(3);

	if (!result)
		result = stack.push(4);
})

//int test_StackPop() {
BFTEST_UNIT_FUNC(test_StackPop, 1,  {
	Stack<int> stack;

	result = stack.push(1);

	if (!result)
		result = stack.push(2);

	if (!result)
		result = stack.push(3);

	if (!result)
		result = stack.push(4);

	if (!result) {
		if (stack.top() != 4) {
			printf("%d != 4\n", stack.top());
			result = 1;
		}
	}

	if (!result)
		result = stack.pop();

	if (!result) {
		if (stack.top() != 3) {
			printf("%d != 3\n", stack.top());
			result = 2;
		}
	}
})

//int test_StackTop() {
BFTEST_UNIT_FUNC(test_StackTop, 1,  {
	Stack<int> stack;

	result = stack.push(1);

	if (!result)
		result = stack.push(2);

	if (!result)
		result = stack.push(3);

	if (!result)
		result = stack.push(4);

	if (!result) {
		int top = stack.top();

		if (top != 4) {
			result = 8;
			printf("%d != 4\n", top);
		}
	}
})

//int test_StackSize() {
BFTEST_UNIT_FUNC(test_StackSize, 1,  {
	Stack<int> stack;

	if (!stack.empty()) {
		result = 1;
		printf("Stack should be empty\n");
	}

	if (!result)
		result = stack.push(1);

	if (!result)
		result = stack.push(2);

	if (!result)
		result = stack.push(3);

	if (!result)
		result = stack.push(4);

	if (stack.empty()) {
		result = 2;
		printf("Stack should not be empty\n");
	}

	if (stack.size() != 4) {
		result = 3;
		printf("Size: %d != 4\n", stack.size());
	}
})

//int test_LoadAndUnload() {
BFTEST_UNIT_FUNC(test_LoadAndUnload, 1,  {
	srand(time(0));
	const int size = (2 << 8);
	int * array = (int *) malloc(sizeof(int) * size);
	Stack<int> stack;

	for (int i = 0; i < size; i++) {
		array[i] = rand();
		stack.push(array[i]);
	}

	if (stack.size() != size) result = 1;

	if (result == 0) {
		for (int i = size - 1; i >= 0; i--) {
			if (stack.top() != array[i]) {
				result = 2;
				break;
			}
			stack.pop();
		}
	}

	BFFree(array);
})

//int test_LoadAndUnloadStrings() {
BFTEST_UNIT_FUNC(test_LoadAndUnloadStrings, 1,  {
	const int size = (2 << 16);
	char ** array = (char **) malloc(sizeof(char *) * size);
	Stack<char *> stack;
	for (int i = 0; i < size; i++) {
		char uuidstr[kBFStringUUIDStringLength];
		strcpy(uuidstr, "uuid");
		array[i] = BFStringCopyString(uuidstr);
		if (array[i] == NULL) {
			result = i;
			break;
		}
		stack.push(array[i]);
	}

	if (stack.size() != size) result = 1;
	if (result == 0) {
		for (int i = size - 1; i >= 0; i--) {
			stack.pop();
			if (strlen(array[i]) == 0) {
				result = 2;
				break;
			}
		}
	}

	for (int i = 0; i < size; i++ ){
		BFFree(array[i]);
	}
	
	BFFree(array);
})

BFTEST_COVERAGE_FUNC(stack_tests, {
	BFTEST_LAUNCH(test_StackInit);
	BFTEST_LAUNCH(test_StackPush);
	BFTEST_LAUNCH(test_StackPop);
	BFTEST_LAUNCH(test_StackTop);
	BFTEST_LAUNCH(test_StackSize);
	BFTEST_LAUNCH(test_LoadAndUnload);
	BFTEST_LAUNCH(test_LoadAndUnloadStrings);
})

#endif // STACK_TESTS_HPP

