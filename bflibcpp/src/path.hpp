/**
 * author: brando
 * date: 1/5/24
 */

#ifndef PATH_HPP
#define PATH_HPP

#include <limits.h>
#include "object.hpp"

#ifdef LINUX
#include <linux/limits.h>
#endif 

namespace BF {

class Path : public Object {
protected:
	Path(const char * path);
	virtual ~Path();
	
	/**
	 * Will return the entire path to file
	 */
	const char * path();

	/**
	 * returns absolut path
	 *
	 * similar to realpath
	 */
	const char * abspath();

private:
	/** 
	 * Holds the full path
	 *
	 * When this function is initialized, the path is 
	 * set the real path
	 */
	char _path[PATH_MAX];

	char _reserved[PATH_MAX];
};

};

#endif // PATH_HPP

