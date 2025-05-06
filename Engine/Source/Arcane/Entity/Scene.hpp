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

	extern ComponentID sNextID;

	template<typename _Type>
	ComponentID GetComponentID() {
		static ComponentID id = sNextID++;
		return id;
	}

	class ComponentPool {
	public:
		ComponentPool() : mData(nullptr), mElementSize(0) { }
		ComponentPool(size_t componentSize) : mElementSize(componentSize) {
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
			mMainEntity = AR_INVALID_ENTITY_ID;
			mMaxEntityID = 0;
		}

		~Scene() { }

		EntityID CreateEntity() {
			const EntityID entity = (EntityID)mAvailableEntities._Find_first();
			if (entity >= AR_MAX_ENTITIES) return -1;

			mAvailableEntities.set(entity, false);
			mMaxEntityID = std::max(mMaxEntityID, entity);
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

		bool IsEntity(EntityID entity) const {
			if (entity == AR_INVALID_ENTITY_ID) return false;
			return !mAvailableEntities.test(entity);
		}

		template<typename ..._Types>
		EntityID FindEntity(EntityID start = 0) const {
			const uint32_t componentCount = sizeof...(_Types);
			const ComponentID componentIDs[] = { GetComponentID<_Types>()... };
			std::bitset<AR_MAX_COMPONENTS> mask;
			for (uint32_t i = 1; i < componentCount; i++) {
				mask.set(componentIDs[i]);
			}

			bool found = false;
			EntityID entity = start;

			while (entity <= mMaxEntityID) {
				if (!IsEntity(entity)) continue;
				if (mask == (mask & mEntities[entity])) {
					found = true;
					break;
				}
				entity++;
			}

			return found ? entity : AR_INVALID_ENTITY_ID;
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
		_Type &AddComponent(EntityID entity, _Type &&data) {
			AR_ASSERT(entity != AR_INVALID_ENTITY_ID, "Entity is invalid");
			AR_ASSERT(!mAvailableEntities.test(entity), "Entity %u does not exist", entity);
			
			const ComponentID componentId = GetComponentID<_Type>();
			AR_ASSERT(componentId < AR_MAX_COMPONENTS, "ComponentID %u is invalid", componentId);

			if (mPools.size() <= componentId) {
				mPools.emplace_back(sizeof(_Type));
			}

			_Type *component = new (mPools[componentId].GetComponent(entity)) _Type(std::forward<_Type>(data));
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

		template<typename ..._Types>
		bool HasComponents(EntityID entity) const {
			AR_ASSERT(entity != AR_INVALID_ENTITY_ID, "Entity is invalid");
			AR_ASSERT(!mAvailableEntities.test(entity), "Entity %u does not exist", entity);

			const uint32_t componentCount = sizeof...(_Types);
			const ComponentID componentIDs[] = { GetComponentID<_Types>()... };
			std::bitset<AR_MAX_COMPONENTS> mask;
			for (uint32_t i = 0; i < componentCount; i++) {
				mask.set(componentIDs[i]);
			}

			return mask == (mask & mEntities[entity]);
		}

		template<typename _Type>
		void RemoveComponent(EntityID entity) {
			AR_ASSERT(entity != AR_INVALID_ENTITY_ID, "Entity is invalid");
			AR_ASSERT(!mAvailableEntities.test(entity), "Entity %u does not exist", entity);

			const ComponentID componentId = GetComponentID<_Type>();
			AR_ASSERT(componentId < AR_MAX_COMPONENTS, "ComponentID %u is invalid", componentId);

			if (!mEntities[entity].test(componentId)) return;
			_Type *component = (_Type*)mPools[componentId].GetComponent(entity);
			std::destroy_at(component);
			mPools[componentId].ResetComponent(entity);

			mEntities[entity].reset(componentId);
		}

		inline std::bitset<AR_MAX_COMPONENTS> &GetEntityComponentMask(EntityID entity) {
			AR_ASSERT(entity != AR_INVALID_ENTITY_ID, "Entity is invalid");
			AR_ASSERT(!mAvailableEntities.test(entity), "Entity %u does not exist", entity);
			return mEntities[entity];
		}
		
		inline uint32_t GetMaxEntityID() const { return mMaxEntityID; }
		inline uint32_t GetEntityCount() const { return AR_MAX_ENTITIES - mAvailableEntities.count(); }

		inline EntityID GetMainEntity() const { return mMainEntity; }
		inline void SetMainEntity(EntityID entity) { mMainEntity = entity; }

	private:
		std::vector<ComponentPool> mPools;
		std::array<std::bitset<AR_MAX_COMPONENTS>, AR_MAX_ENTITIES> mEntities;
		std::bitset<AR_MAX_ENTITIES> mAvailableEntities;
		uint32_t mMaxEntityID;

		EntityID mMainEntity;
	};

	void SetCurrentScene(Scene *scene);
	Scene *GetCurrentScene();


}