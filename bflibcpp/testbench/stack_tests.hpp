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

void stack_tests(int * pass, int * fail) {
	int p = 0, f = 0;
	
	INTRO_TEST_FUNCTION;

	if (!test_StackInit()) p++;
	else f++;

	if (!test_StackPush()) p++;
	else f++;

	if (!test_StackPop()) p++;
	else f++;

	if (!test_StackTop()) p++;
	else f++;

	if (!test_StackSize()) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // STACK_TESTS_HPP

