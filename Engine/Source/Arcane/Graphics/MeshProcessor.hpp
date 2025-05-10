#pragma once

#include "Loader.hpp"
#include <vector>
#include <initializer_list>

namespace Arcane {

	class MeshProcessor {
	public:
		MeshProcessor();
		MeshProcessor(const std::initializer_list<MeshProcess> &processes);
		~MeshProcessor();

		MeshData &Process(MeshData &data) const;

	private:
		std::vector<MeshProcess> mProcesses;
	};

}