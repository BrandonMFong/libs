/**
 * author: brando
 * date: 4/20/25
 */

#include "defer.hpp"

using namespace BF;

__Defer__::__Defer__(std::function<void(void)> cb) : _cb(cb) { }

__Defer__::~__Defer__() {
	this->_cb();
}

