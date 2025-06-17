#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Math/Matrix4.hpp>
#include <Arcane/Math/Vector4.hpp>

#include <Arcane/Graphics/Base/Texture.hpp>
#include <Arcane/Graphics/Base/Mesh.hpp>

#define AR_SHADOW_MAP_WIDTH 1024
#define AR_SHADOW_MAP_HEIGHT 1024

#define AR_SAMPLE_COUNT 1

namespace Arcane {

	struct ObjectData {
		Matrix4 Model;
		Matrix4 MVP;
		Vector4 Position;
	};

	struct CameraData {
		Matrix4 Projection;
		Matrix4 View;
		Vector4 Position;
		float NearPlane, FarPlane;
	};

	struct RenderSubmission {
		Matrix4 Model;
		Vector3 Position;
		Mesh Mesh;
		Texture AlbedoMap;
		Texture NormalMap;
		Texture MetallicMap;
		Texture RoughnessMap;
		Texture AmbientOcclusionMap;
	};

	struct LightData {
		struct {
			Vector4 Position;
			Color Color;
			float Intensity;
			float _PADDING0[3];
		} PointLights[32];
		uint32_t PointLightCount;
		float _PADDING0[3];
		struct {
			Vector4 Direction;
			Color Color;
		} DirectionalLight;
	};

	struct PostProcessSettings {
		float Gamma;
		float Exposure;
	};

	struct ShadowPassData {
		Matrix4 LightProjection;
		Matrix4 LightView;
	};

}