#pragma once

#include <Arcane/Core.hpp>

#if 0
#define AR_FUNCTION_TIMER() ::Arcane::ScopedTimer(__FUNCTION__)
#define AR_NAMED_SCOPED_TIMER(name) ::Arcane::ScopedTimer(name)
#else
#define AR_FUNCTION_TIMER()
#define AR_NAMED_SCOPED_TIMER(name)
#endif

namespace Arcane {

	void UpdateTime();
	float GetDeltaTime();
	float Now();

	uint64_t GetCurrentTimeMillis();

	class Timer {
	public:
		Timer() { mTime = 0; }
		~Timer() { }

		inline void Start() { mTime = GetCurrentTimeMillis(); }
		inline void End() { mTime = GetCurrentTimeMillis() - mTime; }
		inline uint64_t GetElapsedTime() const { return mTime; }

	private:
		uint64_t mTime;
	};

	class ScopedTimer {
	public:
		ScopedTimer(const std::string &name) : mName(name) {
			mTime = GetCurrentTimeMillis();
		}

		~ScopedTimer() {
			uint64_t elapsed = GetCurrentTimeMillis() - mTime;
			std::printf("%s: %llums\n", mName.c_str(), elapsed);
		}

	private:
		uint64_t mTime;
		std::string mName;
	};
	
}