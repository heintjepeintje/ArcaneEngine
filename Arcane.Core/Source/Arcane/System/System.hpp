#pragma once

#include <Arcane/Core/Core.hpp>

namespace Arcane {

	class ISystem {
	public:
		virtual ~ISystem() = default;

		virtual B8 initialize() = 0;
		virtual void shutdown() = 0;
	};

}
