#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Graphics/Camera.hpp>
#include <Arcane/Graphics/Color.hpp>

namespace Arcane {

	class RenderCamera {
	public:
		RenderCamera(const Camera3D &camera, float gamma, float exposure) : mCamera(camera), mGamma(gamma), mExposure(exposure) { }
		RenderCamera(const Camera3D &camera) : mCamera(camera) { }
		~RenderCamera() { }

		inline Camera3D &GetCamera() { return mCamera; }
		inline Camera3D GetCamera() const { return mCamera; }
		inline void SetCamera(const Camera3D &camera) { mCamera = camera; }

		inline float GetGamma() const { return mGamma; }
		inline void SetGamma(float gamma) { mGamma = gamma; }
		
		inline float GetExposure() const { return mExposure; }
		inline void SetExposure(float exposure) { mExposure = exposure; }
		
		inline Color &GetBackgroundColor() { return mBackgroundColor; }
		inline Color GetBackgroundColor() const { return mBackgroundColor; }
		inline void SetBackgroundColor(const Color &color) { mBackgroundColor = color; }

		inline void SetActive(bool active) { mIsActive = active; }
		inline bool IsActive() const { return mIsActive; }

	private:
		Camera3D mCamera;
		float mGamma = 2.2f;
		float mExposure = 1.0f;
		Color mBackgroundColor = Color::Black();
		bool mIsActive = true;
	};

}