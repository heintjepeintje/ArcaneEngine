#pragma once

#include <Arcane/Core.hpp>
#include "Image.hpp"

namespace Arcane {

	enum class AttachmentType {
		None = 0,
		Color,
		Depth, 
		Stencil, 
		DepthStencil
	};

	struct Attachment {
		AttachmentType Type;
		ImageFormat Format;
		uint32_t Samples;
	};

}