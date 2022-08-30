/**
 * author: Brando
 * date: 6/30/22
 */

#include "array_tests.hpp"
#include "list_tests.hpp"
#include "file_tests.hpp"
#include "rbtree_tests.hpp"

int main() {
	int pass = 0, fail = 0, tp = 0, tf = 0;

	printf("\n---------------------------\n");
	printf("\nStarting Array tests...\n\n");
	array_tests(&pass, &fail);
	
	printf("\nPass: %d\n", pass);
	printf("Fail: %d\n", fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;

	printf("\n---------------------------\n");
	printf("\nStarting File tests...\n\n");
	file_tests(&pass, &fail);

	printf("\nPass: %d\n", pass);
	printf("Fail: %d\n", fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;
	
	printf("\n---------------------------\n");
	printf("\nStarting Red Black Tree tests...\n\n");
	rbtree_tests(&pass, &fail);

	printf("\nPass: %d\n", pass);
	printf("Fail: %d\n", fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;
	
	printf("\n---------------------------\n");
	printf("\nStarting List tests...\n\n");
	list_tests(&pass, &fail);
	
	printf("\nPass: %d\n", pass);
	printf("Fail: %d\n", fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;

	printf("\n---------------------------\n\n");

	return 0;
}

