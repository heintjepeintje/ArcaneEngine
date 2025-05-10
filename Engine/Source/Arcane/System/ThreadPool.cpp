#include "ThreadPool.hpp"

namespace Arcane {

	int32_t ThreadFunc(void *data) {
		std::vector<Task> &tasks = *(std::vector<Task>*)data;
		while (true) {
			if (tasks.empty()) {
				Thread::Sleep(100);
				continue;
			}
			Task &t = tasks.front();
			t.ThreadIndex = Thread::GetCurrent().GetID();
			t.Result = t.Func(t.Data);
			t.Completed = true;
		}
	}

	ThreadPool::ThreadPool(uint32_t count) {
		mNextTaskID = 0;
		mThreads.reserve(count);
		for (uint32_t i = 0; i < count; i++) {
			mThreads.push_back(Thread::Create(ThreadFunc, &mTasks));
		}
	}

	ThreadPool::~ThreadPool() {
		for (uint32_t i = 0; i < mThreads.size(); i++) {
			mThreads[i].Terminate(-1);
		}
	}

	TaskID ThreadPool::AddTask(TaskFunc func, void *data) {
		mTasks.emplace_back(
			mNextTaskID++,
			func,
			data
		);
	}

	void *ThreadPool::AwaitTask(TaskID id) {
		uint32_t index = UINT32_MAX;

		for (uint32_t i = 0; i < mTasks.size(); i++) {
			if (mTasks[i].ID == id) index = i;
		}

		while (!mTasks[index].Completed) {
			Thread::Sleep(10);
		}

		return mTasks[index].Result;
	}


}