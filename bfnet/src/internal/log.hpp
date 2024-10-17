/**
 * author: brando
 * date: 10/17/24
 */

#ifndef BF_NET_INTERNAL_LOG_HPP
#define BF_NET_INTERNAL_LOG_HPP

namespace BF {
namespace Net {
namespace Log {

/**
 * thread safe writing using the callback
 * defined in SetCallback
 */
void Write(const char * format, ...);

}
}
}

#endif // BF_NET_INTERNAL_LOG_HPP

