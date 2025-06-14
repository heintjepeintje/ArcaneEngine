#pragma once

#include <Arcane/Core.hpp>

namespace Arcane {

	struct PacketHeader {
		uint32_t Type;
		uint64_t Timestamp;
		uint32_t Size;
	};

	struct Packet {
		PacketHeader Header;
		void *Data;
	};

}