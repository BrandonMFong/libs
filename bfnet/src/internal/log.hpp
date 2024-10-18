/**
 * author: brando
 * date: 10/17/24
 */

#ifndef BF_NET_INTERNAL_LOG_HPP
#define BF_NET_INTERNAL_LOG_HPP

#if defined(DEBUG) && !defined(TESTING)
#define BFNetLogDebug(...) BF::Net::Log::Write(__VA_ARGS__)
#else
#define BFNetLogDebug(...)
#endif

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

