#pragma once
#include <cstdint>

struct ImageStructure {
	int width;
	int height;
	uint32_t* pixeles;

	~ImageStructure() {
		delete[] pixeles;
	}
};