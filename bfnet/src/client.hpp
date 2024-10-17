/**
 * author: brando
 * date: 1/24/24
 */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "socket.hpp"

extern "C" {
#include <bflibc/typethreadid.h>
}

namespace BF {
namespace Net {

class Client : public Socket {
public:
	Client();
	virtual ~Client();
	const char mode() const;
	bool isRunning() const;
protected:
	static void init(void * in);
	int _start();
	int _stop();
	BFThreadAsyncID inStreamThreadID() const;
private:
};

}
}

#endif // CLIENT_HPP

