#pragma once

#include <Arcane/Core.hpp>

namespace Arcane {

	class Color {
	public:
		inline static Color White() { return Color(1.0f, 1.0f, 1.0f, 1.0f); }
		inline static Color Gray() { return Color(0.5f, 0.5f, 0.5f, 1.0f); }
		inline static Color Black() { return Color(0.0f, 0.0f, 0.0f, 1.0f); }
		inline static Color Red() { return Color(1.0f, 0.0f, 0.0f, 1.0f); }
		inline static Color Green() { return Color(0.0f, 1.0f, 0.0f, 1.0f); }
		inline static Color Blue() { return Color(0.0f, 0.0f, 1.0f, 1.0f); }
		inline static Color Cyan() { return Color(0.0f, 1.0f, 1.0f, 1.0f); }
		inline static Color Yellow() { return Color(1.0f, 1.0f, 0.0f, 1.0f); }
		inline static Color Magenta() { return Color(1.0f, 0.0f, 1.0f, 1.0f); }

		inline static Color FromHex(uint32_t hex) {
			float r = (float)((hex >> 24) & 0xFF);
			float g = (float)((hex >> 16) & 0xFF);
			float b = (float)((hex >> 8)  & 0xFF);
			float a = (float)((hex >> 0)  & 0xFF);
			return Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
		}

	public:
		Color() : R(0.0f), G(0.0f), B(0.0f), A(1.0f) { }
		Color(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
		Color(float r, float g, float b) : R(r), G(g), B(b), A(1.0f) { }
		~Color() = default;

	public:
		float R, G, B, A;
	};

}