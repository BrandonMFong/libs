/**
 * author: brando
 * date: 3/13/24
 */

#ifndef ENVELOPE_HPP
#define ENVELOPE_HPP

#include <bflibcpp/object.hpp>
#include "buffer.hpp"

namespace BF {
namespace Net {

class Connection;
class Socket;

class Envelope : public BF::Object {
	friend class Socket;
public:
	Envelope(Connection * sc, size_t bufsize);
	virtual ~Envelope();

	SocketBuffer * buf();
	Connection * connection();
private:
	Connection * _sc;
	SocketBuffer _buf;
};

}
}

#endif // ENVELOPE_HPP

