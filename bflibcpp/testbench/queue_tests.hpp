/**
 * author: Brando
 * date: 1/24/24
 */

#ifndef QUEUE_TESTS_HPP
#define QUEUE_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include <queue.hpp>

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

int test_queueinit() {
	UNIT_TEST_START;
	int result = 0;

	Queue<int> q;

	UNIT_TEST_END(!result, result);
	return result;
}

int test_loadandunloadQueue(void) {
	UNIT_TEST_START;
	int result = 0;

	Queue<int> q;
	const int max = 2 << 10;
	for (int i = 0; i < max; i++) {
		q.push(i);
	}

	if (q.size() != max) result = 2;

	if (!result) {
		for (int i = 0; i < max; i++) {
			int n = q.front();
			if (n != i) {
				result = 1;
				break;
			}
			q.pop();
		}
	}

	UNIT_TEST_END(!result, result);
	return result;
}

//void queue_tests(int * pass, int * fail) {
TEST_COVERAGE_FUNC(queue_tests) {
	TEST_COVERAGE_START;

	LAUNCH_TEST(test_queueinit, p, f);
	LAUNCH_TEST(test_loadandunloadQueue, p, f);

	TEST_COVERAGE_END;
}

#endif // QUEUE_TESTS_HPP 

