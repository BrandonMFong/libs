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
	int result = 0;

	Queue<int> q;

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_loadandunloadQueue(void) {
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

void queue_tests(int * pass, int * fail) {
	int p = 0, f = 0;
	
	INTRO_TEST_FUNCTION;

	LAUNCH_TEST(test_queueinit, p, f);
	LAUNCH_TEST(test_loadandunloadQueue, p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // QUEUE_TESTS_HPP 

