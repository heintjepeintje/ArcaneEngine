#include "Socket.hpp"

namespace Arcane {

	Socket Socket::Create() {
		return Socket(NativeSocket::Create());
	}

}