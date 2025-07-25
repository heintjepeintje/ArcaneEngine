#include "Shader.hpp"

#include <Arcane/Util/FileUtil.hpp>

#include <sstream>

namespace Arcane {

	void CompileShader(const GraphicsContext &context, const std::filesystem::path &path, const std::filesystem::path &output) {
		AR_ENGINE_INFO("Compiling shader: {}", path.string());

		std::stringstream cmd;
		cmd << "glslc ";

#ifdef _DEBUG
		cmd << "-g -D_DEBUG ";
#endif

		if (path.extension() == ".vsh" || path.extension() == ".vert") {
			cmd << "-fshader-stage=vertex ";
		} else if (path.extension() == ".fsh" || path.extension() == ".frag") {
			cmd << "-fshader-stage=fragment ";
		} else {
			AR_ASSERT(false, "Shader file extension is unknown: {}", path.extension().string());
		}

		cmd << path << " -o " << output;

		int code = std::system(cmd.str().c_str());
		AR_ASSERT(code == 0, "Shader compilation failed with code {}: ({})", code, cmd.str());
	}


}