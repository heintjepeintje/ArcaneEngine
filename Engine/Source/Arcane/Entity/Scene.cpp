#include "Scene.hpp"

namespace Arcane {

	static Scene *sCurrentScene = new Scene();

	void SetCurrentScene(Scene &scene) {
		sCurrentScene = &scene;
	}

	Scene &GetCurrentScene() {
		return *sCurrentScene;
	}

}