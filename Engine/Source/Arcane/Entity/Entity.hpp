#pragma once

#include <Arcane/Core.hpp>
#include "Scene.hpp"

namespace Arcane {

	class Entity {
	public:
		Entity();
		Entity(Scene &scene);
		Entity(EntityID id, Scene &scene);
		~Entity() = default;

		template<typename _Type, typename ..._Args>
		_Type &Add(_Args &&...args) {
			AR_ASSERT(mScene != nullptr, "Scene of entity %u is nullptr", mID);
			return mScene->AddComponent<_Type>(mID, std::forward<_Args>(args)...);
		}

		template<typename _Type>
		_Type &Add(_Type &&t) {
			AR_ASSERT(mScene != nullptr, "Scene of entity %u is nullptr", mID);
			return mScene->AddComponent<_Type>(mID, std::forward<_Type>(t));
		}

		template<typename _Type>
		void Remove() {
			AR_ASSERT(mScene != nullptr, "Scene of entity %u is nullptr", mID);
			mScene->RemoveComponent<_Type>(mID);
		}

		template<typename _Type>
		bool Has() const {
			AR_ASSERT(mScene != nullptr, "Scene of entity %u is nullptr", mID);
			return mScene->HasComponent<_Type>(mID);
		}

		template<typename _Type>
		_Type &Get() {
			AR_ASSERT(mScene != nullptr, "Scene of entity %u is nullptr", mID);
			return mScene->GetComponent<_Type>(mID);
		}

		template<typename _Type>
		const _Type &Get() const {
			AR_ASSERT(mScene != nullptr, "Scene of entity %u is nullptr", mID);
			return mScene->GetComponent<_Type>(mID);
		}

		inline Scene &GetScene() const { return *mScene; }
		inline EntityID GetID() const { return mID; }

	private:
		Scene *mScene;
		EntityID mID;
	};

}