#pragma once

#include <Arcane/Core.hpp>
#include "Color.hpp"

namespace Arcane {

	class PointLight {
	public:
		PointLight(const Color &color, float intensity)
			: Color(color), Intensity(intensity) { }
		~PointLight() { }

	public:
		Color Color;
		float Intensity;
	};

	class DirectionalLight {
	public:
		DirectionalLight(const Color &color)
			: Color(color) { }
		
	public:
		Color Color;
	};

}