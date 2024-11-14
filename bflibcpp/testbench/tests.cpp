/**
 * author: Brando
 * date: 6/30/22
 */

#include "cpplib_tests.hpp"
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
#include "vector_tests.hpp"

int main() {
	TEST_SUITE_START;

	TEST_SUITE_LAUNCH(atomic_tests);
	TEST_SUITE_LAUNCH(file_tests);
	TEST_SUITE_LAUNCH(bintree_tests);
	TEST_SUITE_LAUNCH(rbtree_tests);
	TEST_SUITE_LAUNCH(dictionary_tests);
	TEST_SUITE_LAUNCH(string_tests);
	TEST_SUITE_LAUNCH(stack_tests);
	TEST_SUITE_LAUNCH(queue_tests);
	TEST_SUITE_LAUNCH(time_tests);
	TEST_SUITE_LAUNCH(object_tests);
	TEST_SUITE_LAUNCH(data_tests);
	TEST_SUITE_LAUNCH(list_tests);
	TEST_SUITE_LAUNCH(vector_tests);
	TEST_SUITE_LAUNCH(array_tests);

	TEST_SUITE_END;

	return 0;
}

