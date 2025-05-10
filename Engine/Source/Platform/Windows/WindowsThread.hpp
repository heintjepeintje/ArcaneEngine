#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Native/NativeThread.hpp>
#include <winbase.h>
#include <processthreadsapi.h>

namespace Arcane {

	class WindowsThread : public NativeThread {
	friend class NativeThread;
	public:
		WindowsThread(ThreadFunc func, void *data);
		WindowsThread(HANDLE thread, DWORD id);
		~WindowsThread();

		virtual void Start() override;
		virtual void Suspend() override;
		virtual void Terminate(int32_t code) override;
		virtual int32_t GetExitCode() const override;
		virtual ThreadID GetID() const override;

	private:
		HANDLE mThread;
		DWORD mID;
	};

}