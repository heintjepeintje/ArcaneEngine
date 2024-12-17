#pragma once

#include <Arcane/Core.hpp>

namespace Arcane {

	enum class ImageFormat {
		None = 0,
		RGB,
		RGBA,
		Depth,
		Stencil,
		DepthStencil
	};

}