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

int test_StackInit() {
	int result = 0;

	Stack<int> stack;

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_StackPush() {
	int result = 0;

	Stack<int> stack;

	result = stack.push(1);

	if (!result)
		result = stack.push(2);

	if (!result)
		result = stack.push(3);

	if (!result)
		result = stack.push(4);

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_StackPop() {
	int result = 0;

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

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_StackTop() {
	int result = 0;

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

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_StackSize() {
	int result = 0;

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

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_LoadAndUnload() {
	UNIT_TEST_START;
	int result = 0;
	srand(time(0));
	const int size = (2 << 23);
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
	UNIT_TEST_END(!result, result);
	return result;
}

int test_LoadAndUnloadStrings() {
	UNIT_TEST_START;
	int result = 0;

	const int size = (2 << 16);
	char ** array = (char **) malloc(sizeof(char *) * size);
	Stack<char *> stack;
	for (int i = 0; i < size; i++) {
		char uuidstr[kBFStringUUIDStringLength];
		//BFStringGetRandomUUIDString(uuidstr);
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

	UNIT_TEST_END(!result, result);
	return result;
}

void stack_tests(int * pass, int * fail) {
	int p = 0, f = 0;
	
	INTRO_TEST_FUNCTION;

	LAUNCH_TEST(test_StackInit, p, f);
	LAUNCH_TEST(test_StackPush, p, f);
	LAUNCH_TEST(test_StackPop, p, f);
	LAUNCH_TEST(test_StackTop, p, f);
	LAUNCH_TEST(test_StackSize, p, f);
	LAUNCH_TEST(test_LoadAndUnload, p, f);
	LAUNCH_TEST(test_LoadAndUnloadStrings, p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // STACK_TESTS_HPP

