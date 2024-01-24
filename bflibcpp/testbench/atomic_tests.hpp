/**
 * author: Brando
 * date: 1/24/24
 */

#ifndef ATOMIC_TESTS_HPP
#define ATOMIC_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include <atomic.hpp>
#include <unistd.h>

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

typedef struct {
	Atomic<double> d;
} AtomicInit;

int test_atomicinit() {
	int result = 0;

	Atomic<int> a;
	Atomic<int> b(5);
	Atomic<int> c = 10;
	AtomicInit sa;
	sa.d = 15;

	PRINT_TEST_RESULTS(!result);
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
	int result = 0;

	Atomic<int> a = 10;
	BFThreadAsyncID tid0 = BFThreadAsync(test_atomisetandget_callback, &a);
	BFThreadAsyncID tid1 = BFThreadAsync(test_atomisetandget_callback, &a);

	while (a.get() < 1024) {}

	PRINT_TEST_RESULTS(!result);
	return result;
}

void atomic_tests(int * pass, int * fail) {
	int p = 0, f = 0;
	
	INTRO_TEST_FUNCTION;

	LAUNCH_TEST(test_atomicinit, p, f);
	LAUNCH_TEST(test_atomisetandget, p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // ATOMIC_TESTS_HPP

