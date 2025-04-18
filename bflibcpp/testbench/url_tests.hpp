/**
 * author: Brando
 * date: 4/9/25
 */

#ifndef URL_TESTS_HPP
#define URL_TESTS_HPP

#include <url.hpp>
#include <list.hpp>
#include <string.hpp>
#include "cpplib_tests.hpp"

using namespace BF;

void __testing() {

	URL url = "hello/world.v1.txt";
	url = "hello/world.v2.txt";

}

BFTEST_UNIT_FUNC(test_urlinit, 2 << 10, {
	URL url0(__FILE__);

	URL * url1 = new URL(__FILE__);
	BFRelease(url1);

	String file = __FILE__;
	URL url2(file);
	//__testing();
	/*
	URL url = "hello/world.v1.txt";
	url = "hello/world.v2.txt";
	*/
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

BFTEST_UNIT_FUNC(test_urlStandardPath, 2 << 10, {
	URL url = "/home/test/Downloads/../hello_world.txt";
	BF_ASSERT(strcmp(url.standardPath(), "/home/test/hello_world.txt") == 0, "'%s'->'%s'", url.path(), url.standardPath());
	if (BFTEST_UNIT_FUNC_ITR == 0) {
		BFTestPrint("'%s'->'%s'", url.path(), url.standardPath());
	}
})

BFTEST_UNIT_FUNC(test_urlComponents, 2 << 10, {
	const int testsize = 2;
	const char * paths[testsize] = {
		"/home/test/Downloads/hello_world.txt",
		"/home/test/Downloads/../hello_world.txt"
	};

	int componentCounts[testsize] = {4, 5};
	for (int i = 0; i < testsize; i++) {
		URL url = paths[i];
		if (BFTEST_UNIT_FUNC_ITR == 0) {
			BFTestPrint("absolute path = '%s'", url.abspath());
		}

		const List<String> comps = url.components();
		BF_ASSERT(comps.size() == componentCounts[i]);
		for (const String & comp : comps) {
			if (BFTEST_UNIT_FUNC_ITR == 0) {
				BFTestPrint("component = '%s'", comp.cString());
			}
		}
	}
})

BFTEST_UNIT_FUNC(test_urlCheckPathIsSubPath, 2 << 10, {
	URL url0("hello/world");
	URL url1("hello/world/name.txt");
	BF_ASSERT(url1.isSubPath(url0));

	URL url2 = "hello/world/../name.txt";
	BF_ASSERT(!url2.isSubPath(url0));
	
	URL url3 = "hello/world/fake/../name.txt";
	BF_ASSERT(url3.isSubPath(url0));

	BF_ASSERT(!url0.isSubPath(url1));
	BF_ASSERT(!url0.isSubPath(url2));
	BF_ASSERT(!url0.isSubPath(url3));
})

BFTEST_UNIT_FUNC(test_urlAbsoluteURL, 2 << 10, {
	URL url = __FILE__;
	URL absurl = url.absURL();
	BF_ASSERT(!strcmp(url.abspath(), absurl.path()));
})

BFTEST_UNIT_FUNC(test_urlStandardURL, 2 << 10, {
	URL url = __FILE__;
	URL stdurl = url.standardURL();
	BF_ASSERT(!strcmp(url.standardPath(), stdurl.path()));
})

BFTEST_COVERAGE_FUNC(url_tests, {
	BFTEST_LAUNCH(test_urlinit);
	BFTEST_LAUNCH(test_urlextension);
	BFTEST_LAUNCH(test_urldirectory);
	BFTEST_LAUNCH(test_urlleaf);
	BFTEST_LAUNCH(test_urlname);
	BFTEST_LAUNCH(test_urlabspath);
	BFTEST_LAUNCH(test_urlappend);
	BFTEST_LAUNCH(test_urlStandardPath);
	BFTEST_LAUNCH(test_urlComponents);
	BFTEST_LAUNCH(test_urlCheckPathIsSubPath);
	BFTEST_LAUNCH(test_urlAbsoluteURL);
	BFTEST_LAUNCH(test_urlStandardURL);

})

#endif // URL_TESTS_HPP

