#pragma once
#include <vector>

#include "Sprite.h"
#include "Duck.h"
#include "screen_sprite_defines.h"


//#include "checkML.h"

#define TRIGGERS_MIN 0.5  //Valor minimo para aceptar el input de L2 y R2

class Controller;

class DuckGroup
{
private:

	/**
	* recibe los bordes de colision en el siguiente orden :
	* 0 - leftBoundary
	* 1 - rightBoundary
	* 2 - topBoundary
	* 3 - bottonBoundary
	*
	* en caso de no recibirlo, se inicializan los bordes de colision con los bordes de la ventana
	*/
	int collisionBounds[N_EDGES];

	uint8_t size;

	float ducksScale;

	std::vector<Duck> obj; //vector de patos

	void spawnDucks(int i, float scale);

	Sprite** blueSprites;
	Sprite** redSprites;
	Sprite** blackSprites;
	Sprite** pointsSprites;
	Sprite** markPoints;

	int points;
	int shootsLeft;

	bool keyDown;
	float tiempo;

public: 
	DuckGroup(uint8_t, Sprite* (&_bS)[NUM_SPRITES_DUCK], Sprite* (&_rS)[NUM_SPRITES_DUCK], Sprite* (&_blS)[NUM_SPRITES_DUCK], Sprite* (&_pS)[3], Sprite* (&_xS)[10], int colBounds[N_EDGES] = {0});

	void update(float deltaTime);

	void createDucks();

	void handleEvents(Controller controller, float pointerX, float pointerY);

	void draw();

};

