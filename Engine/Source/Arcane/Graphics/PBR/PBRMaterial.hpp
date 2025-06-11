#pragma once

#include <Arcane/Graphics/Base/Texture.hpp>
#include <Arcane/Graphics/Color.hpp>

namespace Arcane {

	class PBRMaterial {
	public:
		PBRMaterial() { }
		PBRMaterial(const Texture &albedo, const Texture &normal, const Texture &metallic, const Texture &roughness, const Texture &ao) 
			: AlbedoMap(albedo), NormalMap(normal), MetallicMap(metallic), RoughnessMap(roughness), AmbientOcclusionMap(ao) { }
		~PBRMaterial() { }

	public:
		Texture AlbedoMap;
		Texture NormalMap;
		Texture MetallicMap;
		Texture RoughnessMap;
		Texture AmbientOcclusionMap;
	};

}