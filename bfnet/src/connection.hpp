/**
 * author: brando
 * date: 2/26/24
 */

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <bflibcpp/object.hpp>
#include <bflibcpp/atomic.hpp>
#include <bflibcpp/data.hpp>
#include <uuid/uuid.h>

namespace BF {
namespace Net {
	class Socket;
	class Client;
}
}

namespace BF {
namespace Net {

class Connection : public BF::Object {
	friend class Socket;
	friend class Server;
	friend class Client;

public:
	static void ReleaseConnection(Connection * sc);

	/**
	 * if true, then we are now able to send data
	 * to remote user
	 */
	bool isready() const;

	/**
	 * true if the connection is still valid
	 */
	bool isactive() const;
	
	/**
	 * queues up data to be sent
	 *
	 * data : data to be sent.  this data is copied. Caller still owns
	 * size : size of data buffer
	 */
	int queueData(const BF::Data * buf);

	/**
	 * see Socket::mode
	 */
	const char mode() const;

	/**
	 * returns uuid
	 */
	void getuuid(uuid_t uuid) const;

	/**
	 * returns socket type [SOCK_STREAM, SOCK_DGRAM, ...]
	 *
	 * returns -1 if the type could not get sent. I don't 
	 * know if -1 is used amongst the socket types
	 */
	int type() const;

	/**
	 * closes socket descriptor
	 */
	void closeConnection();

private:

	/**
	 * sktref : reference to socket
	 */
	Connection(int sd, Socket * sktref);
	virtual ~Connection();

	int sendData(const BF::Data * buf);

	/**
	 * Reads bytes from socket
	 *
	 * Uses Socket::_cbprogress to allow the implementer 
	 * to control data flow
	 */
	int recvData(BF::Data * buf);

	/**
	 * uniquely ids the connection
	 */
	uuid_t _uuid;
	
	/// socket descriptor
	BF::Atomic<int> _sd;

	/**
	 * true if communication is ready to be made with
	 * our _sd
	 */
	BF::Atomic<bool> _isready;

	/**
	 * reference to socket
	 */
	Socket * _sktref;
};

}
}

#endif // CONNECTION_HPP

