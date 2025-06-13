#pragma once

#include <Arcane/Core.hpp>

namespace Arcane {

	class NativeSocket {
	public:
		static Ref<NativeSocket> Create();

	public:
		NativeSocket() = default;
		virtual ~NativeSocket() = default;

		virtual void Connect(const std::string &addr, uint32_t port) = 0;
		virtual void Bind(const std::string &addr, uint32_t port) = 0;
		virtual void Close() = 0;

		virtual void Listen(uint32_t backlog) = 0;
		virtual Ref<NativeSocket> Accept() = 0;

		virtual void Send(const void *data, size_t size) = 0;
		virtual size_t Receive(void *buffer, size_t size) = 0;

		virtual bool IsValid() const = 0;
	};

}