#include "Mesh.hpp"

namespace Arcane {

	Mesh Mesh::Create(const GraphicsContext &context) {
		return Mesh(NativeMesh::Create(
			context.GetNativeContext()
		));
	}

}