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

int main() {
	int pass = 0, fail = 0;
	float tp = 0, tf = 0;

	printf("\n---------------------------\n");

	array_tests(&pass, &fail);
	printf("[+ %d, - %d]\n", pass, fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;

	file_tests(&pass, &fail);
	printf("[+ %d, - %d]\n", pass, fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;
	
	list_tests(&pass, &fail);
	printf("[+ %d, - %d]\n", pass, fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;
	
	bintree_tests(&pass, &fail);
	printf("[+ %d, - %d]\n", pass, fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;

	rbtree_tests(&pass, &fail);
	printf("[+ %d, - %d]\n", pass, fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;
	
	dictionary_tests(&pass, &fail);
	printf("[+ %d, - %d]\n", pass, fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;
	
	string_tests(&pass, &fail);
	printf("[+ %d, - %d]\n", pass, fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;
	
	stack_tests(&pass, &fail);
	printf("[+ %d, - %d]\n", pass, fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;
	
	queue_tests(&pass, &fail);
	printf("[+ %d, - %d]\n", pass, fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;

	time_tests(&pass, &fail);
	printf("[+ %d, - %d]\n", pass, fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;

	atomic_tests(&pass, &fail);
	printf("[+ %d, - %d]\n", pass, fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;

	printf("Grade - %.2f%% (%d/%d)\n", (tp / (tp + tf)) * 100, (int) tp, (int) (tp + tf));

	return 0;
}

