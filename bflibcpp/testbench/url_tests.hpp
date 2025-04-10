/**
 * author: Brando
 * date: 4/9/25
 */

#ifndef URL_TESTS_HPP
#define URL_TESTS_HPP

#include <url.hpp>
#include "cpplib_tests.hpp"

using namespace BF;

BFTEST_UNIT_FUNC(test_urlinit, 2 << 10, {
	URL url0(__FILE__);

	URL * url1 = new URL(__FILE__);
	BFRelease(url1);
})

BFTEST_UNIT_FUNC(test_urlextension, 2 << 10, {
	URL url0("hello.txt");
	BF_ASSERT(!strcmp(url0.extension(), "txt"));

	URL url1("hello");
	BF_ASSERT(!strcmp(url1.extension(), ""), "url1.ext=%s", url1.extension());
})

BFTEST_UNIT_FUNC(test_urldirectory, 2 << 10, {
	URL url0("hello/world.txt");
	BF_ASSERT(!strcmp(url0.directory(), "hello"));
})

BFTEST_UNIT_FUNC(test_urlleaf, 2 << 10, {
	URL url0("hello/world.txt");
	BF_ASSERT(!strcmp(url0.leaf(), "world.txt"));
})

BFTEST_UNIT_FUNC(test_urlname, 2 << 10, {
	URL url0("hello/world.txt");
	BF_ASSERT(!strcmp(url0.name(), "world"));
})

BFTEST_UNIT_FUNC(test_urlabspath, 2 << 10, {
	URL url0("hello/world.txt");
	BF_ASSERT(strlen(url0.abspath()));
	URL url1(__FILE__);
	BF_ASSERT(strlen(url0.abspath()));
	if (BFTEST_UNIT_FUNC_ITR == 0) {
		BFTestPrint("'%s'->'%s'", url0.path(), url0.abspath());
		BFTestPrint("'%s'->'%s'", url1.path(), url1.abspath());
	}
})

BFTEST_UNIT_FUNC(test_urlappend, 2 << 10, {
	URL url0("hello/world");
	URL url1("hello/world/name.txt");
	URL url2("hello/world/names.txt");
	url0.append("name.txt");
	BF_ASSERT(url0 == url1);
	BF_ASSERT(url0 != url2);
})

BFTEST_COVERAGE_FUNC(url_tests, {
	BFTEST_LAUNCH(test_urlinit);
	BFTEST_LAUNCH(test_urlextension);
	BFTEST_LAUNCH(test_urldirectory);
	BFTEST_LAUNCH(test_urlleaf);
	BFTEST_LAUNCH(test_urlname);
	BFTEST_LAUNCH(test_urlabspath);
	BFTEST_LAUNCH(test_urlappend);
})

#endif // URL_TESTS_HPP

