#pragma once

#include <Arcane/Core.hpp>

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
	
}