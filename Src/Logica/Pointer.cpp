#include "Pointer.h"
#include "Sprite.h"
#include "Controller.h"
#include "Input.h"
#include <iostream>
#include "Duck.h"

Pointer::Pointer(Vector2D p, Sprite* spr, float scale) 
{
	pointer_position = p;
	
	sprite = spr;

	wWidth = Renderer::Instance()->getWidth();
	wHeight = Renderer::Instance()->getHeight();

	scaleX = scaleY = scale;

	movX = 0;
	movY = 0;
}

void Pointer::draw()
{
	sprite->Draw(pointer_position.first, pointer_position.second, scaleX, scaleY);
}

void Pointer::update(float deltaTime)
{
	pointer_position.first += movX * SPEED * scaleX*deltaTime;
	pointer_position.second += movY * SPEED * scaleY*deltaTime;
	movX = 0;
	movY = 0;

	int spriteHeight = sprite->getImg()->getHeight(); //En este caso me da igual coger altura o anchura porqeu son iguales. 

	//Comprobamos que el puntero no se salga de pantalla 

	//Comprobacion horizontal
	if (pointer_position.first < -spriteHeight / 2 * scaleX) pointer_position.first = -spriteHeight / 2 * scaleX;
	else if (pointer_position.first > wWidth - ((spriteHeight * scaleX) / 2)) pointer_position.first = wWidth - ((spriteHeight * scaleX) / 2);

	//Comprobacion vertical
	if (pointer_position.second < -spriteHeight / 2 * scaleY) pointer_position.second = -spriteHeight / 2 * scaleY;
	else if (pointer_position.second > wHeight - ((spriteHeight * scaleY) / 2)) pointer_position.second = wHeight - ((spriteHeight * scaleY) / 2);
}

void Pointer::handleEvents(Controller controller)
{
	movX += controller.gyroscopeAngularVelocityX;
	movY += controller.gyroscopeAngularVelocityY;

	movX += controller.leftStickX;
	movY += controller.leftStickY;
}