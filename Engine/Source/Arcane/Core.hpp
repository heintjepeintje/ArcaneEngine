#pragma once

#include <memory>
#include <string>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

#ifdef TRACY_ENABLE
#	define TRACY_ONLY_LOCALHOST 1
#	include <tracy/Tracy.hpp>
#	define AR_PROFILE_FUNCTION() ZoneScopedN(__PRETTY_FUNCTION__)
#	define AR_PROFILE_SCOPE(name) ZoneScopedN(name)
#	define AR_PROFILE_FRAME_START() FrameMarkStart(nullptr)
#	define AR_PROFILE_FRAME_END() FrameMarkEnd(nullptr)
#else 
#	define AR_PROFILE_FUNCTION()
#	define AR_PROFILE_SCOPE(name)
#	define AR_PROFILE_FRAME()
#endif

#ifdef _DEBUG
#	define AR_ASSERT(x, ...) if (!(x)) { std::fprintf(stderr, __VA_ARGS__); __debugbreak(); }
#else
#	define AR_ASSERT(x, ...)
#endif

#define AR_BIT(x) (1 << x)

#define AR_PTR_ADD(ptr, offset) ((void*)((uintptr_t)ptr + offset))

#define AR_STRCAT(a, b) a##b

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
		Ref() { }

		Ref(RefBlock<_Type> *block) {
			mBlock = block;
			mBlock->Count++;
		}

		Ref(_Type *ptr) {
			mBlock = new RefBlock<_Type>();
			mBlock->Pointer = ptr;
			mBlock->Count = 1;
		}

		Ref(const Ref<_Type> &other) {
			if (other.mBlock == nullptr) return;
			mBlock = other.mBlock;
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