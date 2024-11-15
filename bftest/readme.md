# BFTest Framework

Example main file:
```
#include <bftest.h>

BFTEST_UNIT_FUNC(test_foo1, 1, {
	int error = foo1();
	BF_ASSERT(error == 0);
})

BFTEST_UNIT_FUNC(test_foo2, 1, {
	int error = foo2();
	BF_ASSERT(error == 0);
})

BFTEST_UNIT_FUNC(test_foo3, 1, {
	int error = foo3();
	BF_ASSERT(error == 0);
})

BFTEST_COVERAGE_FUNC(example_tests, {
	BFTEST_LAUNCH(test_foo1);
	BFTEST_LAUNCH(test_foo2);
	BFTEST_LAUNCH(test_foo3);
})

BFTEST_SUITE_FUNC({
	BFTEST_SUITE_LAUNCH(example_tests);
})
```

All binary and headers are located in the `bin` folder. 
