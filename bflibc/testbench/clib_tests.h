/**
 * author: Brando
 * date: 6/30/22
 */

#ifndef CLIB_TESTS_H
#define CLIB_TESTS_H

#define INTRO_TEST_FUNCTION printf("Running %s:\n", __func__)
#define PRINT_TEST_RESULTS(result) \
	if (result) {system("printf \"[\033[0;32m Pass \033[0m] \"");}\
	else {system("printf \"[\033[0;31m Fail \033[0m] \"");}\
	printf("%s\n", __func__)

/**
 * each function should take no params and return 0 on success
 */
#define LAUNCH_TEST(unit_test_function, pass_counter, fail_counter) \
	if (!unit_test_function()) pass_counter++; \
	else fail_counter++;

#endif // CLIB_TESTS_H

