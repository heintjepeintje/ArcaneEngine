#pragma once

#include <stdint.h>
#include <memory>
#include <string>
#include <cstdio>
#include <cstdlib>

#define AR_ASSERT(x, ...) if (!(x)) { std::fprintf(stderr, __VA_ARGS__); __debugbreak(); }