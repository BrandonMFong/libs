/**
 * author: brando
 * date: 4/9/25
 *
 * improvement to File & Path
 */

#ifndef URL_HPP
#define URL_HPP

#include "object.hpp"

#ifdef LINUX
#include <linux/limits.h>
#endif
#include <limits.h>

namespace BF {

class URL : public BF::Object {
public:
	URL(const char * path);
	virtual ~URL();

	// returns: original path
	const char * path() const;

	// returns: absolute path from root
	const char * abspath() const;

	// returns: the last component of the path
	const char * leaf() const;

	// returns: file extension if exists
	const char * extension() const;

	// returns: basename without extension
	const char * name() const;

	// returns: path without current leaf
	const char * directory() const;

	void append(const char * suffix);

private:
	char _path[PATH_MAX];
	mutable char _reserved[PATH_MAX];

public:
	bool operator==(const URL & other) const;
	bool operator!=(const URL & other) const;
};

}

#endif // URL_HPP

