#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Graphics/Base/GraphicsContext.hpp>

namespace Arcane {

	void CompileShader(GraphicsAPI api, const std::filesystem::path &path, const std::filesystem::path &output);

}