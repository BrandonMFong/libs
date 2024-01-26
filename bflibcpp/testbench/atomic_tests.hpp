/**
 * author: Brando
 * date: 1/24/24
 */

#ifndef ATOMIC_TESTS_HPP
#define ATOMIC_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include <atomic.hpp>
#include <unistd.h>
#include <queue.hpp>

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

typedef struct {
	Atomic<double> d;
} AtomicInit;

int test_atomicinit() {
	UNIT_TEST_START;
	int result = 0;

	Atomic<int> a;
	Atomic<int> b(5);
	Atomic<int> c = 10;
	AtomicInit sa;
	sa.d = 15;

	UNIT_TEST_END(!result, result);
	return result;
}

void test_atomisetandget_callback(void * in) {
	Atomic<int> * a = (Atomic<int> *) in;
	while (a->get() < 1024) {
		int i = a->get();
		a->set(++i);
		usleep(500);
	}
}

int test_atomisetandget() {
	UNIT_TEST_START;
	int result = 0;

	Atomic<int> a = 10;
	BFThreadAsyncID tid0 = BFThreadAsync(test_atomisetandget_callback, &a);
	BFThreadAsyncID tid1 = BFThreadAsync(test_atomisetandget_callback, &a);

	while (a.get() < 1024) {}

	BFThreadAsyncIDDestroy(tid0);
	BFThreadAsyncIDDestroy(tid1);

	UNIT_TEST_END(!result, result);
	return result;
}

int test_atomicqueue() {
	UNIT_TEST_START;
	int result = 0;

	Atomic<Queue<int>> q;
	const int max = 2 << 10;
	for (int i = 0; i < max; i++) {
		q.get().push(i);
	}

	if (q.get().size() != max) {
		result = 2;
		printf("%d != %d\n", q.get().size(), max);
	}

	if (!result) {
		for (int i = 0; i < max; i++) {
			int n = q.get().front();
			if (n != i) {
				result = 1;
				break;
			}
			q.get().pop();
		}
	}

	UNIT_TEST_END(!result, result);

	return result;
}

int test_atomicvaluechange() {
	UNIT_TEST_START;
	int result = 0;

	Atomic<int> a;
	long max = (long) 2 << 18;
	while (!result && max) {
		srand(time(0));
		int val = rand();
		a.set(val);
		if (a.get() != val) result = (int) max;
		max--;
	}

	UNIT_TEST_END(!result, result);
	return result;
}

void atomic_tests(int * pass, int * fail) {
	int p = 0, f = 0;
	
	INTRO_TEST_FUNCTION;

	LAUNCH_TEST(test_atomicinit, p, f);
	LAUNCH_TEST(test_atomisetandget, p, f);
	LAUNCH_TEST(test_atomicqueue, p, f);
	LAUNCH_TEST(test_atomicvaluechange, p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // ATOMIC_TESTS_HPP

