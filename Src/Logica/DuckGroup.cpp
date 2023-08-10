#include "DuckGroup.h"
#include "Controller.h"
#include <iostream>
#include "RenderThread.h"

#define TIME_PULSED 0.7f


DuckGroup::DuckGroup(uint8_t size, Sprite* (&_bS)[NUM_SPRITES_DUCK], Sprite* (&_rS)[NUM_SPRITES_DUCK], Sprite* (&_blS)[NUM_SPRITES_DUCK], Sprite* (&_pS)[3], Sprite* (&_xS)[10], int colBounds[N_EDGES])
{
	this->size = size;
	obj.resize(size);
	blueSprites = _bS;
	redSprites = _rS;
	blackSprites = _blS;
	pointsSprites = _pS;
	markPoints = _xS;
	//el bound izquierdo no puede ser igual que el derecho  
	if (colBounds != NULL)
		for (int j = 0; j < N_EDGES; j++)
			collisionBounds[j] = colBounds[j];
	else{
		collisionBounds[LEFT_BOUND] = 0;
		collisionBounds[RIGHT_BOUND] = Renderer::Instance()->getWidth();
		collisionBounds[TOP_BOUND] = 0;
		collisionBounds[BOTTOM_BOUND] = Renderer::Instance()->getHeight();
	}
	keyDown = true;
	shootsLeft = 20000;
	tiempo = 0.0f;
}

void DuckGroup::spawnDucks(int i, float scale) //Crea los patos cuando es necesario
{
	int scaledWidth = (int)(scale * SPRITE_WIDTH);
	int scaledHeight = (int)(scale * SPRITE_WIDTH);

	int spawnRange = Renderer::Instance()->getWidth() - scaledWidth;
	int posX = (int)spawnRange/4 +  rand() % ((int)spawnRange/2); //Spawneamos los patos en la mitad central de la pantalla
	int posY = Renderer::Instance()->getHeight() - scaledHeight-1;

	int c = rand() % 3;
	
	color myColor = color(c);

	//Elegir direccion random
	int random = rand() % (HALF_BOUNCE_ANGLE * 2 + 1);
	float movX = (random - HALF_BOUNCE_ANGLE) / 100.0f;
	float movY = -Duck::GetOtherAxisVel(movX);
	

	if (c == red) {
		obj[i] = Duck((float)posX, (float)posY, movX, movY, myColor, redSprites, scale, (collisionBounds));
	}
	else if (c == blue) {
		obj[i] = Duck((float)posX, (float)posY, movX, movY, myColor, blueSprites, scale, (collisionBounds));
	}
	else obj[i] = Duck((float)posX, (float)posY , movX, movY, myColor, blackSprites, scale, (collisionBounds));


}

void DuckGroup::createDucks() //Crea los patos iniciales
{	
	ducksScale = (Renderer::Instance()->getWidth() / 10.f) / SPRITE_WIDTH;

	for (int i = 0; i < size; i++)	
		spawnDucks(i, ducksScale);
	
}



void DuckGroup::update(float deltaTime) //Llamada al grupo de patos para que se actualicen
{
	tiempo += deltaTime;
	for (int i = 0; i < size; i++)	
		if (!obj[i].update(deltaTime)) { //Si se sale de pantalla crea otro		
			if (obj[i].isFallingDuck()) {
				if ((int)obj[i].getColor() == red) {
					points+=POINTSRED*20;
				}
				else if ((int)obj[i].getColor() == blue) {
					points+=POINTSBLUE*20;
				}
				else
				{
					points+=POINTSBLACK*20;
				}
			}


			spawnDucks(i, ducksScale);
		}

		
	
}

void DuckGroup::handleEvents(Controller controller, float pointerX, float pointerY)
{

	if (keyDown&&(controller.cross || 
		controller.circle ||
		controller.L2 >= TRIGGERS_MIN || 
		controller.R2 >= TRIGGERS_MIN))
	{
		if (shootsLeft>=0) shootsLeft--;
		std::cout << shootsLeft << std::endl;
		keyDown = false;
		for (int i = size-1; i >= 0; i--)		
			if (shootsLeft >= 0) {

				//Si se le dispara al pato devuelve true y deja de comprobar colisiones
				if (obj[i].duckShoot(pointerX, pointerY))
					break;
			}

	}
	else if (!controller.cross &&
		!controller.circle &&
		controller.L2 < TRIGGERS_MIN &&
		controller.R2 < TRIGGERS_MIN) {
		keyDown = true;
	}

	if (controller.square) {
		if (shootsLeft <= 0) {
			shootsLeft = 3;
		}
	}
}

void DuckGroup::draw()//Llamada al grupo de patos para que se pinten
{
	for (int i = 0; i < size; i++) {
		obj[i].draw();
		if (obj[i].isFallingDuck()) {
			pointsSprites[(int)(obj[i].getColor())]->Draw((int)(obj[i].getPosDeadX()), (int)obj[i].getPosDeadY(), ducksScale, ducksScale);
		}
	}
	Command com;
	com.type = DRAW_TEXT;
	com.textInfo.points = points;
	com.textInfo.sprites = markPoints;
	com.textInfo.x = (float)Renderer::Instance()->getWidth() * 17.0f / 40.0f; // Modifica x1 a x
	com.textInfo.y = (float)Renderer::Instance()->getHeight() * 20.5f / 22.5f; // Agrega la variable y
	com.textInfo.w = (float)Renderer::Instance()->getWidth() / (float)40.0f;
	com.textInfo.h = (float)Renderer::Instance()->getHeight() / (float)40.0f;

	RenderThread::Instance()->enqueueCommand(com);

	Command com2;
	com2.type = DRAW_TEXT;
	com2.textInfo.points = shootsLeft;
	com2.textInfo.sprites = markPoints;
	com2.textInfo.x = (float)Renderer::Instance()->getWidth() * 8.0f / 40.0f; // Modifica x1 a x
	com2.textInfo.y = (float)Renderer::Instance()->getHeight() * 9.5f / 22.5f; // Agrega la variable y
	com2.textInfo.w = (float)Renderer::Instance()->getWidth() / (float)40.0f;
	com2.textInfo.h = (float)Renderer::Instance()->getHeight() / (float)40.0f;

	RenderThread::Instance()->enqueueCommand(com2);

	Command com3;
	com3.type = DRAW_TEXT;
	com3.textInfo.points = (int)tiempo;
	com3.textInfo.sprites = markPoints;
	com3.textInfo.x = (float)Renderer::Instance()->getWidth() * 32.0f / 40.0f; // Modifica x1 a x
	com3.textInfo.y = (float)Renderer::Instance()->getHeight() * 9.5f / 22.5f; // Agrega la variable y
	com3.textInfo.w = (float)Renderer::Instance()->getWidth() / (float)40.0f;
	com3.textInfo.h = (float)Renderer::Instance()->getHeight() / (float)40.0f;

	RenderThread::Instance()->enqueueCommand(com3);

}