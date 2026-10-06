#pragma once

#include <Arcane/Core/Core.hpp>
#include "./System.hpp"
#include "./Memory.hpp"
#include <Arcane/Data/Slice.hpp>

namespace Arcane {

	class INetworkingSystem : public ISystem {
	public:
		virtual ~INetworkingSystem() = default;

		virtual B8 initialize() = 0;
		virtual void shutdown() = 0;
	};

	enum class AddressProtocol {
		NONE,
		IPV4,
		IPV6
	};

	enum class TransferProtocol {
		NONE,
		TCP,
		UDP
	};

	struct SocketInfo {
		AddressProtocol address_protocol;
		TransferProtocol transfer_protocol;
	};

	class ISocket {
	public:
		static UniquePtr<ISocket> create(INetworkingSystem& networkingSystem, const SocketInfo& info);

		virtual ~ISocket() = default;

		virtual void connect() = 0;
		virtual void listen(U32 backlog) = 0;
		virtual UniquePtr<ISocket> accept() = 0;

		virtual U32 send(const Slice<U8>& data) const = 0;
		virtual U32 receive(const Slice<U8>& data) const = 0;

		template<typename T>
		inline U32 send(const Slice<T>& data) const { return send(data.reinterpret_as<U8>()); }

		template<typename T>
		inline U32 receive(const Slice<T>& data) const { return receive(data.reinterpret_as<U8>()); }
	};

}