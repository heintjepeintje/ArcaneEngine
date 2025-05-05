#include "Scene.hpp"

namespace Arcane {

	static Scene *sCurrentScene;

	ComponentID sNextID = 0;

	void SetCurrentScene(Scene *scene) {
		sCurrentScene = scene;
	}

	Scene *GetCurrentScene() {
		return sCurrentScene;
	}

}