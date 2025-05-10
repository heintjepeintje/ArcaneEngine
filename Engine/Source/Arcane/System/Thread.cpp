#include "Thread.hpp"

namespace Arcane {

	Thread Thread::Create(ThreadFunc func, void *data) {
		return Thread(NativeThread::Create(func, data));
	}

	void Thread::Exit(int32_t code) {
		NativeThread::Exit(code);
	}

	void Thread::Sleep(uint32_t millis) {
		NativeThread::Sleep(millis);
	}

	void Thread::Await(const Thread &thread, uint32_t max) {
		NativeThread::Await(thread.GetNativeHandle(), max);
	}

	Thread Thread::GetCurrent() {
		return Thread(NativeThread::GetCurrent());
	}

}