#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Math/Vector3.hpp>
#include "Image.hpp"
#include "Color.hpp"

#include <vector>
#include <functional>
#include <array>

namespace Arcane {

	enum class ImageProcess {
		None = 0,
		FlipHorizontal,
		FlipVertical,
		Invert,
	};

	struct ImageData {
		uint32_t Width, Height;
		ImageFormat Format;
		uint8_t *Data;
	};

	ImageData LoadImage(const std::string &path, ImageFormat requestedFormat);
	ImageData LoadImage(const Color &color, ImageFormat format);
	
	void ProcessImage(ImageData &data, ImageProcess process);

}