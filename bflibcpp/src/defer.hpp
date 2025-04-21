/**
 * author: brando
 * date: 4/20/25
 */

#ifndef DEFER_HPP
#define DEFER_HPP

#include <functional>

/**
 * body is expected to be a lamda of the form:
 * ```
 * [] () {
 * 	<code>
 * }
 * ```
 */
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

