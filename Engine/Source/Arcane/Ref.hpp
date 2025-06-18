#pragma once

#include "Log.hpp"

namespace Arcane {

	template<typename _Type>
	struct RefBlock {
		RefBlock() { }
		~RefBlock() { }

		_Type *Pointer;
		uint32_t Count;
	};

	template<typename _Type>
	class Ref {
	public:
		static Ref<_Type> Invalid() { return Ref<_Type>(); }
		
	public:
		Ref() { }

		Ref(RefBlock<_Type> *block) : mBlock(block) {
			mBlock->Count++;
		}

		Ref(_Type *ptr) {
			mBlock = new RefBlock<_Type>();
			mBlock->Pointer = ptr;
			mBlock->Count = 1;
		}

		Ref(const Ref<_Type> &other) : mBlock(other.mBlock) {
			if (mBlock == nullptr) return;
			mBlock->Count++;
		}

		~Ref() {
			if (!mBlock) return;
			mBlock->Count--;
			if (mBlock->Count == 0) {
				delete mBlock->Pointer;
				delete mBlock;
			}
		}

		inline _Type *GetPointer() {
			AR_ASSERT(mBlock, "Reference Block is nullptr");
			return mBlock->Pointer; 
		}
		const _Type *GetPointer() const {
			AR_ASSERT(mBlock, "Reference Block is nullptr");
			return mBlock->Pointer;
		}

		inline RefBlock<_Type> *GetRefBlock() { return mBlock; }
		inline const RefBlock<_Type> *GetRefBlock() const { return mBlock; }

		_Type *operator->() { return GetPointer(); }
		const _Type *operator->() const { return GetPointer(); }
		_Type &operator*() { return *GetPointer(); }
		const _Type &operator*() const { return *GetPointer(); }

		Ref<_Type> &operator=(const Ref<_Type> &other) {
			if (mBlock != nullptr) {
				mBlock->Count--;
				if (mBlock->Count == 0) {
					delete mBlock->Pointer;
					delete mBlock;
				}
			}

			mBlock = other.mBlock;
			mBlock->Count++;

			return *this;
		}

		inline void Drop() {
			if (mBlock == nullptr) return;
			mBlock->Count--;

			if (mBlock->Count != 0) return;
			
			delete mBlock->Pointer;
			delete mBlock;

			mBlock = nullptr;
		}

		inline bool IsValid() const { return mBlock != nullptr && mBlock->Pointer != nullptr; }
		operator bool() const { return IsValid(); }


	private:
		RefBlock<_Type> *mBlock = nullptr; 
	};

	template<typename _RefType, typename ..._Args>
	static Ref<_RefType> CreateRef(_Args &&...args) {
		return Ref<_RefType>(new _RefType(std::forward<_Args>(args)...));
	}

	template<typename _CastType, typename _RefType>
	static Ref<_CastType> CastRef(const Ref<_RefType> &ref) {
		return Ref<_CastType>((RefBlock<_CastType>*)ref.GetRefBlock());
	}

}