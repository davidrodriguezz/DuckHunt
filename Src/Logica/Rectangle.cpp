#include "Rectangle.h"
#include <iostream>



Rectangle::Rectangle()
{

}

Rectangle::Rectangle(float posX, float posY, float xMov, float yMov, int w, int h, color col)
{
	x = posX;
	y = posY;
	height = h;
	width = w;
	c = col;

	wHeight = Renderer::Instance()->getHeight();
	wWidth = Renderer::Instance()->getWidth();

	movX = xMov;
	movY = yMov;


	if (c.r == 255) // red	
		v = 700; 

	else if (c.b) // blue
		v = 500;

	else v = 300;  // black


}

//Actualiza los rectangulos 
//devuelve false si se acabo el tiempo de vida de los rectangulos y han salido de pantalla
bool Rectangle::update(float deltaTime)
{
	life -= deltaTime;

	y += movY * deltaTime * v;
	x += movX * deltaTime * v;
	//CheckColision
	if (life > 0)
	{
		// right boundary
		if (x + width > wWidth)
		{
			int random = rand() % (HALF_BOUNCE_ANGLE * 2 + 1);
			movY = (random - HALF_BOUNCE_ANGLE) / 100.0f;
			movX = -GetOtherAxisVel(movY);
		}
		// left boundary
		else if (x < 0)
		{
			int random = rand() % (HALF_BOUNCE_ANGLE * 2 + 1);
			movY = (random - HALF_BOUNCE_ANGLE) / 100.0f;
			movX = GetOtherAxisVel(movY);
		}
		// bottom boundary
		if (y + height > wHeight)
		{
			
			int random = rand() % (HALF_BOUNCE_ANGLE * 2 + 1);
			movX = (random - HALF_BOUNCE_ANGLE) / 100.0f;
			movY = -GetOtherAxisVel(movX);
		}
		// upper boundary
		else if (y < 0)
		{
			
			int random = rand() % (HALF_BOUNCE_ANGLE * 2 + 1);
			movX = (random - HALF_BOUNCE_ANGLE) / 100.0f;
			movY = GetOtherAxisVel(movX);
		}
		return true;
	}
	//si no tiene vida y se ha salido 
	else
		// right boundary    // left boundary       // bottom boundary		// upper boundary
		if (x > wWidth ||    x + width < 0 ||     y > wHeight + height ||    y + height < 0)		
			return false;		
	
	return true;
}

void Rectangle::render()
{
	Renderer::Instance()->drawRect((int)x, (int)y, width, height, c);
}

inline int Rectangle::GetOtherAxisVel(int vel)
{
	return 1-(std::abs(vel));
}
