#pragma once

#include "Scene.hpp"
#include "Entity.hpp"
#include <iostream>
#include <functional>

namespace Arcane {

	class SceneIterator {
	public:
		SceneIterator(Scene *scene, EntityID entity, const std::bitset<AR_MAX_COMPONENTS> &mask, bool all) 
			: mScene(scene), mEntity(entity), mMask(mask), mAll(all) { }

		Entity operator*() const {
			return Entity(mEntity, *mScene);
		}

		bool operator==(const SceneIterator &other) const {
			return mEntity == other.mEntity && mScene == other.mScene; 
		}

		bool operator!=(const SceneIterator &other) const {
			return mEntity != other.mEntity || mScene != other.mScene;
		}

		SceneIterator &operator++() {
			do {
				mEntity++;
			}  while (mEntity <= mScene->GetMaxEntityID() && !IsValidEntity());
			return *this;
		}

	private:
		bool IsValidEntity() {
			return 
				mScene->IsEntity(mEntity) &&
				(mAll || mMask == (mMask & mScene->GetEntityComponentMask(mEntity)));
		}

	private:
		Scene *mScene;
		EntityID mEntity;
		std::bitset<AR_MAX_COMPONENTS> mMask;
		bool mAll = false;
	};

	template<typename ..._Types>
	class SceneView {
	public:
		SceneView() : SceneView(GetCurrentScene()) { }
		SceneView(Scene &scene) : SceneView(&scene) { }
		SceneView(Scene *scene) : mScene(scene), mMask(0) {
			const uint32_t componentCount = sizeof...(_Types);
			mAll = componentCount == 0;
			if (mAll) return;
			
			ComponentID components[] = { GetComponentID<_Types>()... };
			for (uint32_t i = 0; i < componentCount; i++) {
				mMask.set(components[i], true);
			}

			mCount = 0;
			for (EntityID i = 0; i < mScene->GetMaxEntityID(); i++) {
				if (mMask == (mMask & mScene->GetEntityComponentMask(i))) {
					mCount++;
				}
			}
		}

		inline void ForEach(const std::function<void(Entity e, _Types &...)> &func) {
			for (EntityID i = 0; i < mScene->GetMaxEntityID(); i++) {
				if (!mScene->IsEntity(i)) continue;
				if (mMask != (mMask & mScene->GetEntityComponentMask(i))) continue;
				
				Entity entity(i, *mScene);
				func(entity, mScene->GetComponent<_Types>(i)...);
			}
		}

		inline void ForEach(const std::function<void(_Types &...)> &func) {
			for (EntityID i = 0; i < mScene->GetMaxEntityID(); i++) {
				if (!mScene->IsEntity(i)) continue;
				if (mMask != (mMask & mScene->GetEntityComponentMask(i))) continue;
				
				func(mScene->GetComponent<_Types>(i)...);
			}
		}

		inline Entity FindFirst(const std::function<bool(Entity e, _Types &...)> &condition) {
			for (EntityID i = 0; i < mScene->GetMaxEntityID(); i++) {
				if (!mScene->IsEntity(i)) continue;
				if (mMask != (mMask & mScene->GetEntityComponentMask(i))) continue;
				
				Entity entity(i, *mScene);
				if (condition(entity, entity.Get<_Types>()...)) {
					return entity;
				}
			}
			return Entity(AR_INVALID_ENTITY_ID, *mScene);
		}

		inline SceneIterator begin() {
			EntityID first = 0;
			while (
				first < mScene->GetMaxEntityID() &&
				mMask != (mMask & mScene->GetEntityComponentMask(first)) ||
				!mScene->IsEntity(first))
			{
				first++;
			}
			return SceneIterator(mScene, first, mMask, mAll);
		}

		inline SceneIterator end() {
			return SceneIterator(mScene, mScene->GetMaxEntityID() + 1, mMask, mAll);
		}

		inline uint32_t GetCount() const { return mCount; }
		inline std::bitset<AR_MAX_COMPONENTS> GetComponentMask() const { return mMask; }

	private:
		uint32_t mCount;
		Scene *mScene;
		std::bitset<AR_MAX_COMPONENTS> mMask;
		bool mAll;
	};

}