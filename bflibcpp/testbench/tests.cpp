/**
 * author: Brando
 * date: 6/30/22
 */

#include "array_tests.hpp"
#include "list_tests.hpp"
#include "file_tests.hpp"
#include "rbtree_tests.hpp"
#include "bintree_tests.hpp"
#include "dictionary_tests.hpp"
#include "string_tests.hpp"
#include "stack_tests.hpp"
#include "time_tests.hpp"
#include "queue_tests.hpp"
#include "atomic_tests.hpp"
#include "object_tests.hpp"
#include "data_tests.hpp"

int main() {
	TEST_SUITE_START;

	LAUNCH_TEST_SET(array_tests);
	LAUNCH_TEST_SET(file_tests);
	LAUNCH_TEST_SET(list_tests);
	LAUNCH_TEST_SET(bintree_tests);
	LAUNCH_TEST_SET(rbtree_tests);
	LAUNCH_TEST_SET(dictionary_tests);
	LAUNCH_TEST_SET(string_tests);
	LAUNCH_TEST_SET(stack_tests);
	LAUNCH_TEST_SET(queue_tests);
	LAUNCH_TEST_SET(time_tests);
	LAUNCH_TEST_SET(atomic_tests);
	LAUNCH_TEST_SET(object_tests);
	LAUNCH_TEST_SET(data_tests);

	TEST_SUITE_END;

	return 0;
}

