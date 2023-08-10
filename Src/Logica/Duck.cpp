#include "Duck.h"
#include <iostream>


Duck::Duck(float posX, float posY, float xMov, float yMov, color co, Sprite** _sprites, float scale_, const int(&colBounds)[N_EDGES])
{	
	x = posX;
	y = posY;

	c = co;

	scale = scale_;

	for (int j = 0; j < N_EDGES; j++)
		collisionBounds[j] = colBounds[j];


	//int wHeight = Renderer::Instance()->getHeight();
	int wWidth = Renderer::Instance()->getWidth();

	movX = xMov;
	movY = yMov;

	sprites = _sprites;

	if (c == red) // red
	{
		v = wWidth * 0.37f; //37% de pantalla por seg
		vAnim = REDVEL;
	}
	else if (c == blue) // blue
	{
		v = wWidth * 0.26f; //26% de pantalla por seg
		vAnim = BLUEVEL;
	}
	else {
		v = wWidth * 0.15f; //15% de pantalla por seg // black
		vAnim = BLACKVEL;
	}

	//Ponemos el offset a 0 para poner el inicio de la animaci�n
	offset = 0;

	//como siempre al inicio los patos van hacia arriba al crarse, nos fijamos en la velocidad en X para elegir animacion
	collisionAnimationChange(BOTTOM_BOUND);

	//timer a 0
	timer = 0;
	timerDead = 1;
	isDead = false;
	isFalling = false;
}


void Duck::draw() //Pintado del objeto

{
	sprites[nextAnimation + offset]->Draw((int)x, (int)y, scale, scale);

}

void Duck::collisionAnimationChange(bounds boundCollided) {
	switch (boundCollided) {
		//Como va a ir hacia la izquierda, comprobamos que velocidad tiene en la Y para cambiar el tipo de la animacion seg�n esta variable
		case RIGHT_BOUND:
			if (movY > -ANGLE_STEP && movY < ANGLE_STEP)
				nextAnimation = LEFT * NUM_FRAMES;
			else  nextAnimation = TOPLEFT * NUM_FRAMES;
		break;
		//Como va a ir hacia la derecha, comprobamos que velocidad tiene en la Y para cambiar el tipo de la animacion seg�n esta variable
		case LEFT_BOUND:
			if (movY > -ANGLE_STEP && movY < ANGLE_STEP)
				nextAnimation = RIGHT * NUM_FRAMES;
			else  nextAnimation = TOPRIGHT * NUM_FRAMES;
		break;
		//Como va a ir hacia arriba, comprobamos que velocidad tiene en la X para cambiar el tipo de la animacion seg�n esta variable
		case TOP_BOUND:
			if (movX >= ANGLE_STEP)
				nextAnimation = TOPRIGHT * NUM_FRAMES;
			else  nextAnimation = TOPLEFT * NUM_FRAMES;
		break;
		//Como va a ir hacia abajo, comprobamos que velocidad tiene en la X para cambiar el tipo de la animacion seg�n esta variable
		case BOTTOM_BOUND:
			if (movX > -ANGLE_STEP && movX < ANGLE_STEP)
				nextAnimation = TOP * NUM_FRAMES;
			else if (movX >= ANGLE_STEP)
				nextAnimation = TOPRIGHT * NUM_FRAMES;
			else  nextAnimation = TOPLEFT * NUM_FRAMES;
		break;
	}

}

bool Duck::update(float deltaTime) //Actualizacion del objeto
{
	life -= deltaTime;

	y += movY * deltaTime * v;
	x += movX * deltaTime * v;
	//CheckColision
	if (life > 0 && !isDead)
	{
		// right boundary
		if (x + scale * WIDE_SPRITE_WIDTH > collisionBounds[RIGHT_BOUND] && movX > 0.01f)
		{
			movY = GetRandomVel();
			movX = -GetOtherAxisVel(movY);
			collisionAnimationChange(RIGHT_BOUND);
		}
		// left boundary
		else if (x < collisionBounds[LEFT_BOUND] && movX < 0.01f)
		{
			movY = GetRandomVel();
			movX = GetOtherAxisVel(movY);
			collisionAnimationChange(LEFT_BOUND);
		}
		// bottom boundary
		if (y + scale * SPRITE_HEIGHT > collisionBounds[BOTTOM_BOUND]&& movY > 0.01f)
		{	
			movX = GetRandomVel();
			movY = -GetOtherAxisVel(movX);
			collisionAnimationChange(BOTTOM_BOUND);
		}
		// upper boundary
		else if (y < collisionBounds[TOP_BOUND] && movY < 0.01f)
		{
			movX = GetRandomVel();
			movY = GetOtherAxisVel(movX);
			collisionAnimationChange(TOP_BOUND);
		}
		timer += deltaTime;
		updateDuckAnimation();


		return true;
	}
	else if (isDead) {
		timerDead -= deltaTime;
		if (timerDead <= 0) {
			movY = 1;
			offset = 0;
			isDead = false;
			isFalling = true;
		}
		updateDuckAnimation();
		return true;
	}
	else if (life <= 0) {
			// right boundary          		
		if (x > collisionBounds[RIGHT_BOUND] || 
			// left boundary 
			x + scale * WIDE_SPRITE_WIDTH < collisionBounds[LEFT_BOUND] || 
			// bottom boundary
			y > collisionBounds[BOTTOM_BOUND] || 
			// upper boundary
			y + scale * SPRITE_HEIGHT < collisionBounds[TOP_BOUND])

				return false;

		timer += deltaTime;
		updateDuckAnimation();

		return true;
	}

	return true;
}

bool Duck::duckShoot(float pointerX, float pointerY)
{
	//std::cout << x << "     " << pointerX << "       " << x + (WIDE_SPRITE_WIDTH * scale) << std::endl;
	//std::cout << y << "     " << pointerY << "       " << y + (SPRITE_HEIGHT * scale) << std::endl;

	if (x < pointerX && pointerX < x + (WIDE_SPRITE_WIDTH * scale) && //Comprobar que esta dentro en x
		y < pointerY && pointerY < y + (SPRITE_HEIGHT * scale)&&!isDead&&!isFalling)		 //Comprobar que esta dentro en y
	{
		kill();
		return true;
	}
	else return false;
}



void Duck::updateDuckAnimation() // Comprobar orden de animacion de patos
{
	if (timer > vAnim) {
		timer = vAnim - timer;
		if (!isDead && timerDead == 1) {
			if (frameAnimation + 1 == NUM_FRAMES) {		//cuando es el tercer frame de la animacion, volvemos al segundo frame para que la animacion dure 4 frames
				offset -= 1;
				frameAnimation++;
			}
			else if (frameAnimation == NUM_FRAMES) {			//cuando llega al fin de la animaci�n vuelve al principio de esta
				offset = 0;
				frameAnimation = 0;
			}
			else {										//en este caso, la animaci�n avanza de forma normal
				frameAnimation += 1; offset += 1;
			}

		}
		else if(isFalling) {
				if (offset >= 1) {
					offset = 0;
				}
				else offset = 1;
		}
		
	}
}

void Duck::kill()
{
	posDeadX = x;
	posDeadY = y;
	isDead = true;
	offset = 2;
	nextAnimation = DEAD * NUM_FRAMES;
	movX = 0;
	movY = 0;
	timerDead = 1;
	life = 0;
}
