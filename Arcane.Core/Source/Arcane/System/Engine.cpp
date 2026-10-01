#include "./Engine.hpp"

namespace Arcane {

	B8 Engine::Initialize() {
		B8 MemorySystemInitialized = Platform.GetMemorySystem().Initialize();
		if (!MemorySystemInitialized) return false;

		return true;
	}
	
	void Engine::Shutdown() {
		Platform.GetMemorySystem().Shutdown();
	}

}
