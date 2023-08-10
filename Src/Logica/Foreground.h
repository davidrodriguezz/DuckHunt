#pragma once
#include <utility>
#include <vector>



#define Vector2D std::pair<float,float>

class Sprite;

struct foregroundInfo {
	Vector2D pos;
	int sizeX, sizeY;

	foregroundInfo(Vector2D p, int sX, int sY) : pos(p), sizeX(sX), sizeY(sY) {};
	foregroundInfo() {};
};


class Foreground
{
protected:
	Vector2D pos;

	Sprite* sprite;

	int wHeight;
	int wWidth;

	int width;
	int height;

	float scaleX;
	float scaleY;
public:
	Foreground(Vector2D, Sprite*, float tileSize, float newSizeX, float newSizeY);

	Vector2D GetPos() { return pos; }
	int getHeight() { return height; }
	int getWidth() { return width; }

	void draw();
};

