#pragma once

#include <Arcane/Core.hpp>
#include "Thread.hpp"
#include <vector>

namespace Arcane {

	typedef void *(*TaskFunc)(void *data);

	typedef uint32_t TaskID;

	struct Task {
		Task(TaskID id, TaskFunc func, void *data) : ID(id), Func(func), Result(nullptr), Data(data), ThreadIndex(UINT32_MAX), Completed(false) { }
		~Task() { }

		TaskID ID;
		TaskFunc Func;
		void *Result;
		void *Data;
		uint32_t ThreadIndex;
		bool Completed;
	};

	class ThreadPool {
	public:
		ThreadPool(uint32_t count = 5);
		~ThreadPool();

		TaskID AddTask(TaskFunc func, void *data);
		void *AwaitTask(TaskID id);
	private:
		uint32_t mNextTaskID;
		std::vector<Thread> mThreads;
		std::vector<Task> mTasks;
	};
	
}