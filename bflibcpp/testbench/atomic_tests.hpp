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
	BFThreadAsyncID tid = BFThreadAsyncGetID();
	while (!BFThreadAsyncIsCanceled(tid) && (a->get() < 1024)) {
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

	BFThreadAsyncCancel(tid0);
	BFThreadAsyncCancel(tid1);
	BFThreadAsyncDestroy(tid0);
	BFThreadAsyncDestroy(tid1);

	UNIT_TEST_END(!result, result);
	return result;
}

int test_atomicqueue() {
	UNIT_TEST_START;
	int result = 0;

	Atomic<Queue<int>> q;
	const int max = 2 << 12;
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

void SetValue(void * in) {
	const int max = 2 << 4;
	srand(time(0));
	const int delaytime = rand() % max;
	Atomic<int *> * val = (Atomic<int *> *) in;
	for (int i = 0; i < max; i++) {
		if (i == delaytime)
			usleep(10);

		val->lock();
		int * v = val->unsafeget();
		(*v)++;
		val->unlock();
	}
}

int test_settingvalueonthreads() {
	UNIT_TEST_START;
	int result = 0;

	int max = 2 << 8;
	while (!result && max) {
		int ia = 0;

		Atomic<int *> a(&ia);
		BFThreadAsyncID tid0 = BFThreadAsync(SetValue, &a);
		BFThreadAsyncID tid1 = BFThreadAsync(SetValue, &a);

		// wait for both threads to complete execution
		BFThreadAsyncWait(tid0);
		BFThreadAsyncWait(tid1);

		BFThreadAsyncDestroy(tid0);
		BFThreadAsyncDestroy(tid1);

		const int exp = 2 * (2 << 4); // expected val
		if (ia != exp) {
			printf("\n%d != %d\n", ia, exp);
			result = max;
		}

		max--;
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_equaloverloadop() {
	UNIT_TEST_START;
	int result = 0;

	int max = 2 << 18;
	while (!result && max) {
		srand(time(0));
		int val = rand();
		Atomic<int> a = val;
		Atomic<int> b = val;

		result = a == b ? 0 : max;

		if (!result) {
			a = rand();
			b = rand();
			result = a != b ? 0 : max;
		}

		max--;
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_castingoperator() {
	UNIT_TEST_START;
	int result = 0;

	int max = 2 << 18;
	while (!result && max--) {
		srand(time(0));
		Atomic<int> a = rand();

		int val = a;
		Atomic<int> b = val;
		val = b;
	}

	UNIT_TEST_END(!result, result);
	return result;
}

void test_changebyref(Atomic<bool> & val) {
	val = true;
}

int test_changingvaluebyreference() {
	UNIT_TEST_START;
	int result = 0;

	int max = 2 << 20;
	while (!result && max--) {
		Atomic<bool> val = false;
		if (val) {
			result = 1;
		}

		test_changebyref(val);
		if (!val) {
			result = 2;
		}
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_comparingObjectWithAnother() {
	UNIT_TEST_START;
	int result = 0;

	long long max = (long long) 2 << 40;
	while (!result && max--) {
		Atomic<long long> val = max;
		if ((val != max) || (max != val)) {
			result = max;
		}
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
	LAUNCH_TEST(test_settingvalueonthreads, p, f);
	LAUNCH_TEST(test_equaloverloadop, p, f);
	LAUNCH_TEST(test_castingoperator, p, f);
	LAUNCH_TEST(test_changingvaluebyreference, p, f);
	LAUNCH_TEST(test_comparingObjectWithAnother, p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // ATOMIC_TESTS_HPP

