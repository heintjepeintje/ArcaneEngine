#pragma once

#include <Arcane/Core.hpp>

namespace Arcane {

	class Environment3D {
	public:
		Environment3D() { }
		Environment3D(float exposure, float gamma) : mExposure(exposure), mGamma(gamma) { }
		~Environment3D() { }

		inline float GetExposure() { return mExposure; }
		inline float GetGamma() { return mGamma; }

	private:
		float mExposure;
		float mGamma;
	};

}