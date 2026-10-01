#pragma once

#include <Arcane/Platform/Platform.hpp>
#include "./System.hpp"

namespace Arcane {

	class Engine : public ISystem {
	public:
		Engine() = default;
		~Engine() = default;

		B8 Initialize() override;
		void Shutdown() override;

		inline Platform& GetPlatform() { return Platform; }
	private:
		Platform Platform;	
	};

}
