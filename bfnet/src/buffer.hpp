/**
 * author: brando
 * date: 3/13/24
 */

#ifndef SOCKET_BUFFER_HPP
#define SOCKET_BUFFER_HPP

#include <bflibcpp/object.hpp>
#include <sys/types.h>

namespace BF {
namespace Net {

class SocketConnection;

class SocketBuffer : public BF::Object {
	friend class SocketConnection;
public:
	SocketBuffer(const void * data, size_t size);
	virtual ~SocketBuffer();

	const void * data() const;
	size_t size() const;
private:
	void * _data;
	ssize_t _size;
};

}
}

#endif // SOCKET_BUFFER_HPP

