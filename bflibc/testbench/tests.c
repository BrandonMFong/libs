/**
 * author: Brando
 * date: 6/30/22
 */

#include "coreutils_tests.h"
#include "filesystem_tests.h"
#include "stringutils_tests.h"
#include <stdio.h>

int main() {
	int pass = 0, fail = 0;
	float tp = 0, tf = 0;

	printf("\n---------------------------\n");
	
	coreutils_tests(&pass, &fail);
	printf("[+ %d, - %d]\n", pass, fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;

	filesystem_tests(&pass, &fail);
	printf("[+ %d, - %d]\n", pass, fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;

	stringutils_tests(&pass, &fail);
	printf("[+ %d, - %d]\n", pass, fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;

	printf("Grade - %.2f%% (%d/%d)\n", (tp / (tp + tf)) * 100, (int) tp, (int) (tp + tf));

	return 0;
}

