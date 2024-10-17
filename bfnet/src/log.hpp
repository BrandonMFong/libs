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
 * sets the log callback
 *
 * this is thread safe and can be modified more than once
 */
void SetCallback(void (*cb)(const char * str));

/**
 * thread safe writing using the callback
 * defined in SetCallback
 */
void Write(const char * format, ...);

}
}
}



#endif // BF_NET_LOG_HPP 

