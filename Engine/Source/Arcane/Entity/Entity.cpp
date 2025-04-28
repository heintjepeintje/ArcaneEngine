#include "Entity.hpp"

namespace Arcane {

	Entity::Entity() : Entity(GetCurrentScene()) { }

	Entity::Entity(Scene &scene) {
		mScene = &scene;
		mID = mScene->CreateEntity();
	}

}