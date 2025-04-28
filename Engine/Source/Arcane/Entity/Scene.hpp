#pragma once

#include <Arcane/Core.hpp>
#include <bitset>
#include <queue>

#define AR_MAX_ENTITIES 1024
#define AR_MAX_COMPONENTS 64
#define AR_INVALID_ENTITY_ID UINT32_MAX

namespace Arcane {

	typedef uint32_t EntityID;
	typedef uint32_t ComponentID;

	static ComponentID sNextID = 0;

	template<typename _Type>
	static ComponentID GetComponentID() {
		static ComponentID id = sNextID++;
		return id;
	}

	class ComponentPool {
	public:
		ComponentPool() : mData(nullptr), mElementSize(0) { }
		ComponentPool(size_t componentSize) : mElementSize(componentSize) {
			printf("Creating component pool!\n");
			mData = malloc(componentSize * AR_MAX_ENTITIES);
			std::memset(mData, 0, componentSize * AR_MAX_ENTITIES);
		}

		~ComponentPool() {
			free(mData);
		}

		inline void *GetComponent(uint32_t index) {
			return AR_PTR_ADD(mData, index * (uint32_t)mElementSize);
		}

		inline void ResetComponent(uint32_t index) {
			std::memset(AR_PTR_ADD(mData, index * (uint32_t)mElementSize), 0, mElementSize);
		}

		inline const void *GetData() const { return mData; }
		inline size_t GetElementSize() const { return mElementSize; }

	private:
		void *mData;
		size_t mElementSize;
	};

	class Scene {
	public:
		Scene() {
			mPools.reserve(AR_MAX_COMPONENTS);
			mAvailableEntities.flip();
		}

		~Scene() {

		}

		EntityID CreateEntity() {
			const EntityID entity = (EntityID)mAvailableEntities._Find_first();
			if (entity >= AR_MAX_ENTITIES) return -1;

			mAvailableEntities.set(entity, false);
			return entity;
		}

		void DestroyEntity(EntityID entity) {
			if (!mAvailableEntities.test(entity)) return;

			ComponentID componentId = mEntities[entity]._Find_first();
			while (componentId != AR_MAX_COMPONENTS) {
				mPools[componentId].ResetComponent(entity);
				mEntities[entity].set(componentId, false);
				componentId = mEntities[entity]._Find_next(componentId);
			}

			mAvailableEntities.set(entity);
		}

		template<typename _Type, typename ..._Args>
		_Type &AddComponent(EntityID entity, _Args &&...args) {
			AR_ASSERT(entity != AR_INVALID_ENTITY_ID, "Entity is invalid");
			AR_ASSERT(!mAvailableEntities.test(entity), "Entity %u does not exist", entity);
			
			const ComponentID componentId = GetComponentID<_Type>();
			AR_ASSERT(componentId < AR_MAX_COMPONENTS, "ComponentID %u is invalid", componentId);

			if (mPools.size() <= componentId) {
				mPools.emplace_back(sizeof(_Type));
			}

			_Type *component = new (mPools[componentId].GetComponent(entity)) _Type(std::forward<_Args>(args)...);
			mEntities[entity].set(componentId);

			return *component;
		}

		template<typename _Type>
		_Type &GetComponent(EntityID entity) {
			AR_ASSERT(entity != AR_INVALID_ENTITY_ID, "Entity is invalid");
			AR_ASSERT(!mAvailableEntities.test(entity), "Entity %u does not exist", entity);

			const ComponentID componentId = GetComponentID<_Type>();
			AR_ASSERT(componentId < AR_MAX_COMPONENTS, "ComponentID %u is invalid", componentId);

			AR_ASSERT(mEntities[entity].test(componentId), "Entity %u does not have component %u", entity, componentId);

			_Type *component = (_Type*)mPools[componentId].GetComponent(entity);
			return *component;
		}

		template<typename _Type>
		bool HasComponent(EntityID entity) const {
			AR_ASSERT(entity != AR_INVALID_ENTITY_ID, "Entity is invalid");
			AR_ASSERT(!mAvailableEntities.test(entity), "Entity %u does not exist", entity);

			const ComponentID componentId = GetComponentID<_Type>();
			AR_ASSERT(componentId < AR_MAX_COMPONENTS, "ComponentID %u is invalid", componentId);

			return mEntities[entity].test(componentId);
		}

		template<typename _Type>
		void RemoveComponent(EntityID entity) {
			AR_ASSERT(entity != AR_INVALID_ENTITY_ID, "Entity is invalid");
			AR_ASSERT(!mAvailableEntities.test(entity), "Entity %u does not exist", entity);

			const ComponentID componentId = GetComponentID<_Type>();
			AR_ASSERT(componentId < AR_MAX_COMPONENTS, "ComponentID %u is invalid", componentId);

			if (!mEntities[entity].test(componentId)) return;
			mPools[componentId].ResetComponent(entity);

			mEntities[entity].reset(componentId);
		}

	private:
		std::vector<ComponentPool> mPools;
		std::array<std::bitset<AR_MAX_COMPONENTS>, AR_MAX_ENTITIES> mEntities;
		std::bitset<AR_MAX_ENTITIES> mAvailableEntities;
	};

	void SetCurrentScene(Scene &scene);
	Scene &GetCurrentScene();


}