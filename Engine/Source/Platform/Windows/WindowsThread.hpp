#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Native/NativeThread.hpp>
#include "WindowsCore.hpp"

namespace Arcane {

	class WindowsMutex : public NativeMutex {
	public:
		WindowsMutex();
		~WindowsMutex();

		virtual void Lock() override;
		virtual void Unlock() override;

	private:
		HANDLE mMutex;
	};

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
		inline virtual ThreadID GetID() const override { return ((ThreadID)mID); }

	private:
		HANDLE mThread;
		DWORD mID;
	};

}