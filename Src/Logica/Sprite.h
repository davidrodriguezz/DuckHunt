#pragma once

#include "Image.h"

class Sprite

{
protected:
	int x1, y1, w, h;
	const Image* img; 
public:

	Sprite(int x, int y, int width, int height, const Image* im);
	Sprite(const Image* im);
	void Draw(int px1, int py1, float scaleX, float scaleY);
	const Image* getImg() const { return img; };
	int getX() { return x1; };
	int getY() { return y1; };
	int getW() { return w; };
	int getH() { return h; };
};

