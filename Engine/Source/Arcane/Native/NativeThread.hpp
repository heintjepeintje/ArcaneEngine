#pragma once

#include <Arcane/Core.hpp>

namespace Arcane {

	typedef int32_t ThreadFunc(void *data);
	typedef uint32_t ThreadID;

	class NativeThread {
	public:
		static Ref<NativeThread> Create(ThreadFunc func, void *data);
		static void Exit(int32_t code);
		static void Await(const Ref<NativeThread> &thread, uint32_t seconds = UINT32_MAX);
		static void Sleep(uint32_t millis);

		static Ref<NativeThread> GetCurrent();
		static void Switch();
	
	public:
		NativeThread() { }
		virtual ~NativeThread() { }

		virtual void Start() = 0;
		virtual void Suspend() = 0;
		virtual void Terminate(int32_t code) = 0;
		virtual int32_t GetExitCode() const = 0;
		virtual ThreadID GetID() const = 0;
	};

}