/**
 * author: brando
 * date: 4/20/25
 *
 * abstract:
 * Defer in other languages are used to defer logic when the scope exits. In c++, all object destructors are 
 * called when the memory is going to go out of scope. I leverage this core functionality to 
 * implement a defer block in c++. A class is defined to hold a callback function, which can be
 * defined using c++ lambda syntax, and the class executes callback in its destructor. Using a macro,
 * the use of the class and variable declaration is abracted. All that is needed is a macro call and a
 * lambda block.
 */

#ifndef DEFER_HPP
#define DEFER_HPP

#include <functional>
#define __BF_DEFER_CONCAT_IMPL__( x, y ) x##y
#define __BF_DEFER_MACRO_CONCAT__( x, y ) __BF_DEFER_CONCAT_IMPL__( x, y )

/**
 * body is expected to be a lamda of the form:
 * ```
 * [] () {
 * 	<code>
 * }
 * ```
 */
#define BFDefer(...) \
	BF::__Defer__ __BF_DEFER_MACRO_CONCAT__(__bf_defer_helper__, __COUNTER__)(__VA_ARGS__);

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

