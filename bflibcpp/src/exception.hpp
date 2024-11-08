/**
 * author: brando
 * date: 7/11/24
 */

#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>

namespace BF {

class Exception : public std::exception {
public:
	//Exception(BF::String msg);
	Exception(const char * format, ...);
	virtual ~Exception();

	virtual const char * what() const throw();

private:
	//BF::String _msg;
	char * _msg;
};

}

#endif // EXCEPTION_HPP

