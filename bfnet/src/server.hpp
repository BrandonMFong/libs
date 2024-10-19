/**
 * author: brando
 * date: 1/24/24
 */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "socket.hpp"
#include <bflibcpp/atomic.hpp>

extern "C" {
#include <bflibc/typethreadid.h>
}

namespace BF {
namespace Net {

/**
 * https://handsonnetworkprogramming.com/articles/bind-error-98-eaddrinuse-10048-wsaeaddrinuse-address-already-in-use/
 *
 * the above article describes why reopening a port may error out
 */
class Server : public Socket {
public:
	Server();
	virtual ~Server();
	const char mode() const;
	bool isRunning() const;

protected:
	static void init(void * in);
	int _start();
	int _stop();

	static void pollthread(void * in);

private:
	BF::Atomic<int> _mainSocket;

	/**
	 * polling thread that handles incoming connection
	 * requests
	 */
	BF::Atomic<BFThreadAsyncID> _pollt;
};

}
}

#endif // SERVER_HPP

