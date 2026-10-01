#include <Arcane/System/Engine.hpp>
#include <Arcane/System/Memory.hpp>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <ctime>

namespace Arcane { }

struct Vector2 {
	Arcane::F32 x, y;
};

void PrintHeapInfo(const Arcane::MemoryHeap& heap) {
	Arcane::MemoryHeap::Info heapInfo = heap.GetInfo();
	std::printf("Heap Info:\n\tTotal Size: %zu\n\tAllocated Bytes: %zu\n\tOverhead Bytes: %zu\n\tAllocation Count: %u\n\tEntry Count: %u\n",
			heapInfo.totalSize, heapInfo.allocatedBytes, heapInfo.overheadBytes, heapInfo.allocationCount, heapInfo.entryCount);
}

void RandomizeVector2(Vector2& vector) {
	std::srand(std::time(nullptr));
	vector.x = static_cast<Arcane::F32>(std::rand() % 100);
	vector.y = static_cast<Arcane::F32>(std::rand() % 100);
}

void PrintVector2(const Vector2& vector) {
	std::printf("Vector2: (%f, %f)\n", vector.x, vector.y);
}

int main(int argc, char** argv) {
	Arcane::Engine Engine;
	if (!Engine.Initialize()) return 1;

	Arcane::MemoryHeap mainHeap = Arcane::MemoryHeap(Engine.GetPlatform().GetMemorySystem(), 32);

	Arcane::UniquePtr<Vector2> Vector = Arcane::UniquePtr<Vector2>::Create(mainHeap, 1.0f, 2.0f);
	PrintVector2(*Vector);

	RandomizeVector2(*Vector);
	PrintVector2(*Vector);

	PrintHeapInfo(mainHeap);

	Engine.Shutdown();

	while (true);
	return 0;
}
