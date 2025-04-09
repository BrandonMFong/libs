/**
 * author: Brando
 * date: 4/9/25
 */

#ifndef URL_TESTS_HPP
#define URL_TESTS_HPP

#include <url.hpp>
#include "cpplib_tests.hpp"

using namespace BF;

BFTEST_UNIT_FUNC(test_urlinit, 2 << 5, {
	URL url0(__FILE__);

	URL * url1 = new URL(__FILE__);
	BFRelease(url1);
})

BFTEST_COVERAGE_FUNC(url_tests, {
	BFTEST_LAUNCH(test_urlinit);
})

#endif // URL_TESTS_HPP

