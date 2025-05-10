#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Native/NativeThread.hpp>

namespace Arcane {

	class Thread {
	public:
		static Thread Create(ThreadFunc func, void *data);
		static void Exit(int32_t code);
		static void Sleep(uint32_t millis);
		static void Await(const Thread &thread, uint32_t max = UINT32_MAX);
		static Thread GetCurrent();

	public:
		Thread() { }
		Thread(const Ref<NativeThread> &ref) : mNativeHandle(ref) { }
		~Thread() { }

		inline void Start() { GetNativeHandle()->Start(); }
		inline void Suspend() { GetNativeHandle()->Suspend(); }
		inline void Terminate(int32_t code) { GetNativeHandle()->Terminate(code); }
		inline int32_t GetExitCode() const { return GetNativeHandle()->GetExitCode();}
		inline ThreadID GetID() const { return GetNativeHandle()->GetID(); }

		inline Ref<NativeThread> GetNativeHandle() const {
			AR_ASSERT(mNativeHandle, "");
			return mNativeHandle;
		}

	private:
		Ref<NativeThread> mNativeHandle;
	};

}