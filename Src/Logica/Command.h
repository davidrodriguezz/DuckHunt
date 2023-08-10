#pragma once
#include <cstdint>
#include "Sprite.h"

enum CommandType { CLEAR, PRESENT, DRAW_RECT, DRAW_IMAGE, DRAW_TEXT, EMPTY };



struct DrawImageInfo {
	const Image* img;
	int x1, w, y1, h;
	int tx1, ty1;
	int tx2, ty2;
};

struct DrawTextInfo {
	int points;
	Sprite** sprites;
	int x, y, w, h;
};

struct ClearInfo {
	uint32_t color;
};



struct Command {//Estructura de comandos
	CommandType type;
	union {
		DrawImageInfo drawImageInfo;
		ClearInfo clearInfo;
		DrawTextInfo textInfo;
	};
};