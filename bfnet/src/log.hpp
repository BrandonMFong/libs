/**
 * author: brando
 * date: 10/17/24
 */

#ifndef BF_NET_LOG_HPP
#define BF_NET_LOG_HPP

namespace BF {
namespace Net {
namespace Log {

/**
 * sets the log callback that is used internally BFNet. This callback
 * will be used accordingly based on the release or debug builds. Debug 
 * builds may be more verbose than release
 *
 * this is thread safe and can be modified more than once
 */
void SetCallback(void (*cb)(const char * str));

}
}
}

#endif // BF_NET_LOG_HPP 

