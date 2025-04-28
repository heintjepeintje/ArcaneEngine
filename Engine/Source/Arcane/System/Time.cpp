#include "Time.hpp"

#include <Arcane/Native/NativeTime.hpp>

namespace Arcane {

	uint64_t sLastTime = 0;
	float sDeltaTime = 0.0f;

	void UpdateTime() {
		AR_PROFILE_FUNCTION();
		if (sLastTime == 0) {
			sLastTime = _GetCurrentTimeMillis();
		}
		uint64_t now = _GetCurrentTimeMillis();
		sDeltaTime = (now - sLastTime) / 1000.0f;
		sLastTime = now;
	}

	float Now() {
		return (float)((double)_GetCurrentTimeMillis() / 1000.0);
	}

	float GetDeltaTime() { 
		return sDeltaTime;
	}

	uint64_t GetCurrentTimeMillis() {
		return _GetCurrentTimeMillis();
	}

}