/**
 * author: brando
 * date: 4/20/25
 */

#ifndef DEFER_HPP
#define DEFER_HPP

#include <functional>

#define BFDefer(...) \
	BF::__Defer__ __bf_defer_##__COUNTER__##__(__VA_ARGS__);

namespace BF {
class __Defer__ {
public:
	__Defer__(std::function<void(void)> cb);
	virtual ~__Defer__();

private:
	std::function<void(void)> _cb;
};
}

#endif // DEFER_HPP

