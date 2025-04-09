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

	const char * path() const;
	const char * abspath() const;
	const char * leaf() const;
	const char * extension() const;
	const char * name() const;
	const char * directory() const;

private:
	char _path[PATH_MAX];
	mutable char _reserved[PATH_MAX];
};

}

#endif // URL_HPP

