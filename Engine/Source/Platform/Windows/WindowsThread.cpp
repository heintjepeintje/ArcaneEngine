#include "WindowsThread.hpp"

namespace Arcane {

	void NativeThread::Switch() {
		SwitchToThread();
	}

	static thread_local Ref<NativeThread> sCurrentThread;

	Ref<NativeThread> NativeThread::GetCurrent() {
		if (!sCurrentThread) {
			sCurrentThread = CastRef<NativeThread>(CreateRef<WindowsThread>(
				GetCurrentThread(),
				GetCurrentThreadId()
			));
		}
		return sCurrentThread;
	}

	void NativeThread::Exit(int32_t code) {
		ExitThread(code);
	}

	void NativeThread::Await(const Ref<NativeThread> &thread, uint32_t seconds) {
		Ref<WindowsThread> windowsThread = CastRef<WindowsThread>(thread);

		WaitForSingleObject(windowsThread->mThread, seconds);
	}

	void NativeThread::Sleep(uint32_t millis) {
		::Sleep(millis);
	}

	WindowsMutex::WindowsMutex() {
		mMutex = CreateMutexA(NULL, FALSE, nullptr);
		AR_ASSERT(mMutex != nullptr, "Failed to create mutex: {}", GetWindowsErrorMessageString(GetLastError()));
	}

	WindowsMutex::~WindowsMutex() {
		CloseHandle(mMutex);
	}

	void WindowsMutex::Lock() {
		DWORD result = WaitForSingleObject(mMutex, INFINITE);
		AR_ASSERT(result == WAIT_OBJECT_0, "Failed to lock mutex: {}", GetWindowsErrorMessageString(GetLastError()));
	}

	void WindowsMutex::Unlock() {
		BOOL result = ReleaseMutex(mMutex);
		AR_ASSERT(result, "Failed to unlock mutex: {}", GetWindowsErrorMessageString(GetLastError()));
	}

	WindowsThread::WindowsThread(ThreadFunc func, void *data) {
		mThread = CreateThread(
			nullptr,
			0, /* Use default stack size */
			((LPTHREAD_START_ROUTINE)func),
			data,
			CREATE_SUSPENDED,
			&mID
		);

		AR_ASSERT(mThread != INVALID_HANDLE_VALUE, "Failed to create thread!");
	}

	WindowsThread::WindowsThread(HANDLE thread, DWORD id) : mThread(thread), mID(id) { }

	WindowsThread::~WindowsThread() {
		TerminateThread(mThread, -1);
	}

	void WindowsThread::Start() {
		ResumeThread(mThread);
	}

	void WindowsThread::Suspend() {
		SuspendThread(mThread);
	}

	void WindowsThread::Terminate(int32_t code) {
		TerminateThread(mThread, code);
	}

	int32_t WindowsThread::GetExitCode() const {
		DWORD exitCode = 0;
		GetExitCodeThread(mThread, &exitCode);
		return exitCode;
	}

}