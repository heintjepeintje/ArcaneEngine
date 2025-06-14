#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Native/NativeSocket.hpp>

namespace Arcane {

	class Socket {
	public:
		static Socket Create();

	public:
		Socket() = default;
		Socket(const Ref<NativeSocket> &socket) : mNativeSocket(socket) { }
		~Socket() = default;

		inline void Connect(const std::string &addr, uint32_t port) { GetNativeSocket()->Connect(addr, port); }
		inline void Bind(const std::string &addr, uint32_t port) { GetNativeSocket()->Bind(addr, port); }
		inline void Close() { GetNativeSocket()->Close(); }

		inline void Listen(uint32_t backlog) { return GetNativeSocket()->Listen(backlog); }
		inline Ref<NativeSocket> Accept() { return GetNativeSocket()->Accept();; }

		inline void Send(const void *data, size_t size) { GetNativeSocket()->Send(data, size); }

		template<typename _Type>
		inline void Send(const _Type &data) { GetNativeSocket()->Send(&data, sizeof(_Type)); }

		inline size_t Receive(void *buffer, size_t size) { return GetNativeSocket()->Receive(buffer, size); }

		template<typename _Type>
		inline _Type Receive() {
			_Type t; 
			GetNativeSocket()->Receive(&t, sizeof(_Type)); 
			return t;
		}

		inline size_t GetBytesAvailable() const { return GetNativeSocket()->GetBytesAvailable(); }
		
		inline bool IsValid() const { return GetNativeSocket()->IsValid(); }

		virtual Ref<NativeSocket> GetNativeSocket() const {
			AR_ASSERT(mNativeSocket, "Native socket is nullptr");
			return mNativeSocket;
		}

	private:
		Ref<NativeSocket> mNativeSocket;
	};

}