#pragma once

#include <Arcane/Graphics/Base/Texture.hpp>
#include <Arcane/Graphics/Color.hpp>

namespace Arcane {

	class Material {
	public:
		Material() { }
		Material(const Texture &albedo, const Texture &normal, const Texture &metallic, const Texture &roughness, const Texture &ao) 
			: AlbedoMap(albedo), NormalMap(normal), MetallicMap(metallic), RoughnessMap(roughness), AmbientOcclusionMap(ao) { }
		~Material() { }

	public:
		Texture AlbedoMap;
		Texture NormalMap;
		Texture MetallicMap;
		Texture RoughnessMap;
		Texture AmbientOcclusionMap;
	};

}