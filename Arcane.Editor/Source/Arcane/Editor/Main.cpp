#include <Arcane/System/Engine.hpp>
#include <Arcane/System/Memory.hpp>
#include <Arcane/Data/Buffer.hpp>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <ctime>

namespace Arcane { }

struct Vector2 {
	Arcane::F32 x, y;
};

void randomize_vector2(Vector2& vector) {
	std::srand(std::time(nullptr));
	vector.x = static_cast<Arcane::F32>(std::rand() % 100);
	vector.y = static_cast<Arcane::F32>(std::rand() % 100);
}

void print_vector2(const Vector2& vector) {
	std::printf("Vector2: (%f, %f)\n", vector.x, vector.y);
}

int main(int argc, char** argv) {
	Arcane::Engine engine;
	if (!engine.initialize()) return 1;

	Arcane::Buffer<> buffer = Arcane::Buffer<>::allocate(
		engine.find_heap(Arcane::HEAP_INDEX_MISCELLANIOUS_HEAP), 128);

	Arcane::UniquePtr<Vector2> vector = Arcane::UniquePtr<Vector2>::create(
		engine.find_heap(Arcane::HEAP_INDEX_PHYSICS_HEAP), 1.0f, 2.0f);

	std::printf("Buffer address: %p\n", buffer.data());
	std::printf("Vector address: %p\n", vector.pointer());
	print_vector2(*vector);

	randomize_vector2(*vector);
	print_vector2(*vector);

	engine.shutdown();

	while (true);
	return 0;
}
