/**
 * author: brando
 * date: 1/5/24
 */

#ifndef DIRECTORY_HPP
#define DIRECTORY_HPP

#include "path.hpp"

namespace BF {

class Directory : public Path {
	Directory(const char * path);
	virtual ~Directory();
};

};

#endif // DIRECTORY_HPP

