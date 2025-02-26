/**
 * author: brando
 * date: 2/1/24
 */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <bflibcpp/queue.hpp>
#include <bflibcpp/list.hpp>
#include <bflibcpp/atomic.hpp>
#include <bflibcpp/object.hpp>
#include <stdint.h>

extern "C" {
#include <bflibc/thread.h>
}

#define SOCKET_MODE_SERVER 's'
#define SOCKET_MODE_CLIENT 'c'

#define SOCKET_IP4_ADDR_STRLEN 16

namespace BF {
namespace Net {

class Connection;
class SocketEnvelope;

class Socket : public BF::Object {
	friend class BF::Net::Connection;
public: 
	static Socket * shared();

	/**
	 * creates a server/client socket
	 *
	 * ipaddr: ip address in string format.
	 * 	"0.0.0.0" == INADDR_ANY
	 * 	"127.0.0.1" == INADDR_LOOPBACK
	 */
	static Socket * create(
		const char mode,
		const char * ipaddr,
		uint16_t port,
		int * err
	);

	virtual ~Socket();

	/**
	 * returns: SOCKET_MODE_SERVER if we are a server or
	 * SOCKET_MODE_CLIENT if we are a client
	 */
	virtual const char mode() const = 0;

	int start();
	int stop();

	/**
	 * true if socket is ready to take connections (server) or
	 * is actively connected (client)
	 *
	 * true if start() finished kicking off all
	 * working threads
	 *
	 * is thread safe
	 */
	virtual bool isRunning() const = 0;

	/**
	 * sets callback that gets invoked when incoming data is ready to be handled
	 *
	 * callback owner MUST copy buffer data because the data will be lost when it returns
	 */
	void setInStreamCallback(void (* cb)(BF::Net::SocketEnvelope * envelope));

	/**
	 * see _cbnewconn
	 */
	void setNewConnectionCallback(void (* cb)(BF::Net::Connection * sc));

	/**
	 * buffer length for incoming data
	 *
	 * this is the expected size for all incoming data
	 */
	void setBufferSize(size_t size);

	uint16_t port() const;
	const char * ipaddr() const;

	/**
	 * checks if everything is ready to go before 
	 * calling start()
	 */
	bool isReady() const;

protected:
	Socket();

	// _start & _stop gets called at the start
	// of the start() and stop() respectively
	virtual int _start() = 0;
	virtual int _stop() = 0;

	/**
	 * clients must call this function at the end of _start()
	 *
	 * 'sd' : socket descriptor
	 */
	int startInStreamForConnection(BF::Net::Connection * sc);

	/**
	 * array of devices we are connected to
	 */
	BF::Atomic<BF::List<BF::Net::Connection *>> _connections;

	/**
	 * callback used, if given, when a new connection is made
	 *
	 * sc : keep a record of this if you want to send data to the 
	 * device on the other end.  You do not own memory
	 */
	void (* _cbnewconn)(BF::Net::Connection * sc);

	BF::Atomic<BF::List<BFThreadAsyncID>> _tidin;

private:

	/**
	 * updates the connections list, removing inactive connections
	 */
	void updateConnections();

	/**
	 * call back that gets called in `queueCallback` when it
	 * pops data from in q
	 *
	 * envelope : retain if you plan to use after callback returns
	 */
	void (* _cbinstream)(BF::Net::SocketEnvelope * envelope);

	/**
	 * receives packets and puts them in a queue
	 */
	static void inStream(void * in);

	/**
	 * holds expected buffer size for all incoming and outcoming data
	 *
	 * implementer is responsible for setting this
	 */
	size_t _bufferSize;

	/**
	 * queues outgoing data using send
	 *
	 * each envelope will get released after pop
	 */
	BFLock _outqlock;

	uint16_t _portnum;
	char _ip4addr[SOCKET_IP4_ADDR_STRLEN];
};

}
}

#endif // SOCKET_HPP

