#pragma once

#include <Arcane/Core.hpp>
#include "WindowsCore.hpp"
#include <Arcane/Native/NativeSocket.hpp>

namespace Arcane {

	class WindowsSocket : public NativeSocket {
	public:
		WindowsSocket();
		WindowsSocket(SOCKET socket);
		~WindowsSocket();

		virtual void Connect(const std::string &address, uint32_t port) override;
		virtual void Bind(const std::string &address, uint32_t port) override;
		virtual void Close() override;
		
		virtual void Listen(uint32_t backlog) override;
		virtual Ref<NativeSocket> Accept() override;
		virtual void Send(const void *data, size_t size) override;
		virtual size_t Receive(void *buffer, size_t size) override;

		virtual bool IsValid() const override { return mSocket != INVALID_SOCKET; }

	private:
		SOCKET mSocket;
	};

}