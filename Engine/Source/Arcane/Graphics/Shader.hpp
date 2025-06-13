#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Graphics/Base/GraphicsContext.hpp>

namespace Arcane {

	struct ShaderBinary {
		uint8_t *Data;
		size_t Size;
	};

	void CompileShader(GraphicsAPI api, const std::filesystem::path &path, const std::filesystem::path &output);
	ShaderBinary ReadShaderBinary(const std::filesystem::path &path);

}