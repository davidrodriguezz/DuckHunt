#pragma once
#include <vector>

#include "Renderer.h"
#include "Duck.h"

#define Vector2D std::pair<float,float>

#define SPEED 80 //Velocidad de movimiento del joystick

class Sprite;
class Controller;

class Pointer
{
protected:
	Vector2D pointer_position;

	Sprite* sprite;

	int wHeight;
	int wWidth;

	float movX;
	float movY;

	float scaleX;
	float scaleY;

	
public:
	Pointer(Vector2D p, Sprite* spr, float scale);
	~Pointer() {
		delete sprite;
	}

	void draw();

	void update(float deltaTime);

	void handleEvents(Controller controller);

	int getMidPositionX() { return pointer_position.first + sprite->getImg()->getWidth() / 2; }
	int getMidPositionY() { return pointer_position.second + sprite->getImg()->getHeight() / 2; }
};

