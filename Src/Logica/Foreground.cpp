#include "Foreground.h"
#include "Image.h"
#include "Sprite.h"
#include <iostream>
#include "Renderer.h"

Foreground::Foreground(Vector2D p, Sprite* spr, float tileSize, float newSizeX, float newSizeY)
{
	pos = p;
	sprite = spr;
	const Image* im = spr->getImg();
	width = im->getWidth();
	height = im->getHeight();

	wWidth = Renderer::Instance()->getWidth();
	wHeight = Renderer::Instance()->getHeight();

	//Set sizes
	float widthTiles = width / tileSize;
	scaleX = newSizeX / widthTiles;

	float heigthTiles = height / tileSize;
	scaleY = newSizeY / heigthTiles; //Lost info
}

void Foreground::draw()
{
	sprite->Draw(pos.first, pos.second, scaleX, scaleY);
}

