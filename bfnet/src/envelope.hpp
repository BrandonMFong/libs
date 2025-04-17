/**
 * author: brando
 * date: 3/13/24
 */

#ifndef ENVELOPE_HPP
#define ENVELOPE_HPP

#include <bflibcpp/object.hpp>
#include <bflibcpp/data.hpp>

namespace BF {
namespace Net {

class Connection;
class Socket;

class Envelope : public BF::Object {
	friend class Socket;
public:
	Envelope(Connection * sc, size_t bufsize);
	virtual ~Envelope();

	BF::Data * data();
	Connection * connection();
private:
	Connection * _sc;
	BF::Data _data;
};

}
}

#endif // ENVELOPE_HPP

