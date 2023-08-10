#pragma once

#include "Renderer.h"

//equivalencia de 0-1 a 0-90 , 66 equivale a 60 grados
#define HALF_BOUNCE_ANGLE 66
//4 segundos
#define MAX_LIFE 4

#define RECTANGLE_WIDTH 150
#define RECTANGLE_HEIGHT 100

class Rectangle
{
private:
	int v = 0;
	
	float life = MAX_LIFE;
	
	float x = 0;
	float y = 0;
	int width = 0;
	int height = 0;

	float movX = 0;
	float movY = 0;

	int wHeight = 0;
	int wWidth = 0;

	color c;
public:
	Rectangle();

	Rectangle(float posX, float posY, float xMov,float yMov, int w, int h, color col);

	bool update(float deltaTime);

	void render();

	float getPosX() { return x; }
	float getPosY() { return y; }

	int getWidth() { return width; }
	int getHeight() { return height; }

	color getColor() { return (color)c; }

	static inline int GetOtherAxisVel(int);
};

