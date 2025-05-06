#include "Entity.hpp"

namespace Arcane {

	Entity::Entity() {
		mScene = GetCurrentScene();
		if (mScene == nullptr) {
			mID = AR_INVALID_ENTITY_ID;
		}
		else {
			mID = mScene->CreateEntity();
		}
	}

	Entity::Entity(Scene &scene) {
		mScene = &scene;
		mID = mScene->CreateEntity();
	}

	Entity::Entity(EntityID id, Scene &scene) {
		mScene = &scene;
		mID = id;
	}

}