#pragma once

#include <Arcane/Core/Core.hpp>

namespace Arcane {

	class ISystem {
	public:
		virtual ~ISystem() = default;

		virtual B8 Initialize() = 0;
		virtual void Shutdown() = 0;
	};

}
