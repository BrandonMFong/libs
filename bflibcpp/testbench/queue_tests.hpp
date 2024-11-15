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

//int test_queueinit() {
BFTEST_UNIT_FUNC(test_queueinit, 1,  {
	Queue<int> q;
})

//int test_loadandunloadQueue(void) {
BFTEST_UNIT_FUNC(test_loadandunloadQueue, 1,  {
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
})

//void queue_tests(int * pass, int * fail) {
BFTEST_COVERAGE_FUNC(queue_tests) {
	BFTEST_COVERAGE_START;

	BFTEST_LAUNCH(test_queueinit);
	BFTEST_LAUNCH(test_loadandunloadQueue);

	BFTEST_COVERAGE_END;
}

#endif // QUEUE_TESTS_HPP 

