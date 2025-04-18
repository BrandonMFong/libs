/**
 * author: brando
 * date: 4/9/25
 */

#include "url.hpp"
#include "bflibcpp.hpp"
#include <string.h>
#include <libgen.h>

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

const char * URL::className() const {
	return "BF::URL";
}

URL::URL(const String & path) : URL(path.c_str()) { }

URL::URL(const char * path) : Object() {
	memset(&this->_reserved[0], 0, PATH_MAX);
	if (path) {
		strcpy(this->_path, path);
	} else {
		memset(&this->_path[0], 0, PATH_MAX);
	}
}

URL::~URL() { }

const char * URL::path() const {
	return this->_path;
}

URL URL::absURL() const {
	return URL(this->abspath());
}

const char * URL::abspath() const {
	if (realpath(this->_path, this->_reserved) == NULL) {
		// if above fails, we will just use the path as is
		strcpy(this->_reserved, this->_path);
	}
	return this->_reserved;
}

const char * URL::leaf() const {
	this->_reserved[0] = '\0';
	BFFileSystemPathGetFullname(this->_path, this->_reserved);
	return this->_reserved;
}

const char * URL::extension() const {
	this->_reserved[0] = '\0';
	if (strlen(this->_path)) {
		BFFileSystemPathGetExtension(this->_path, this->_reserved);
	}

	return this->_reserved;
}

const char * URL::name() const {
	this->_reserved[0] = '\0';
	BFFileSystemPathGetName(this->_path, this->_reserved);
	return this->_reserved;
}

const char * URL::directory() const {
	this->_reserved[0] = '\0';
	char t1[PATH_MAX];
	strcpy(t1, this->abspath());
	char * t2 = dirname(t1);

	if (t2) strcpy(this->_reserved, t2);

	return this->_reserved;
}

void URL::append(const String & suffix) {
	this->append(suffix.c_str());
}

void URL::append(const char * suffix) {
	if (!suffix) return;
	if (suffix[0] == '/') suffix++;

	size_t len = strlen(this->_path);
	if (len + 1 == PATH_MAX) return;

	if (this->_path[len - 1] != '/') {
		this->_path[len] = '/';
		this->_path[len + 1] = '\0';
	}

	strcat(this->_path, suffix);
}

bool URL::operator==(const URL & other) const {
	return strcmp(this->_path, other._path) == 0;
}

bool URL::operator!=(const URL & other) const {
	return !(*this == other);
}

const List<String> __URLPathGetComponents(const char * path) {
	List<String> res;
	const char * del = "/";
	char path_copy[PATH_MAX];
	strcpy(path_copy, path);
	char * comp = NULL;
	int i = 0;
	while ((comp = strtok(i++ == 0 ? path_copy : NULL, del))) {
		res.add(comp);
	}

	return res;
}

bool URL::isSubPath(const URL & parent) const {
	URL stdChild(this->standardPath());
	URL stdParent(parent.standardPath());

	const List<String> compsChild = stdChild.components();
	const List<String> compsParent = stdParent.components();
	if (compsChild.size() <= compsParent.size()) {
		return false;
	}

	const List<String>::Node * nc = compsChild.first();
	const List<String>::Node * np = compsParent.first();
	if (!nc || !np) return false;

	while (nc && np) {
		if (nc->object() != np->object()) {
			return false;
		}
		nc = nc->next();
		np = np->next();
	}
	return true;
}

URL URL::standardURL() const {
	return URL(this->standardPath());
}

const char * URL::standardPath() const {
	const List<String> comps = __URLPathGetComponents(this->_path);
	Deque<String> deque;
	for (const String & comp : comps) {
		if (comp == ".") continue;
		else if (comp == "..") {
			deque.pop_back();
		} else {
			deque.push_back(comp);
		}
	}

	String stdpath;
	if (this->_path[0] == '/') {
		stdpath.push_back('/');
	}
	
	while (!deque.empty()) {
		const String & comp = deque.front();
		stdpath.append(comp);

		deque.pop_front();

		if (!deque.empty()) {
			stdpath.push_back('/');
		}
	}

	if (this->_path[strlen(this->_path) - 1] == '/') {
		stdpath.push_back('/');
	}

	strcpy(this->_reserved, stdpath.cString());
	return this->_reserved;
}

const List<String> URL::components() const {
	return __URLPathGetComponents(this->_path);
}

