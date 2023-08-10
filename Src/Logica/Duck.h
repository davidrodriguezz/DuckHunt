#pragma once
#include <utility>

#include "Renderer.h"
#include "Sprite.h"
#include "screen_sprite_defines.h"

//#include "checkML.h"



#define Vector2D std::pair<float,float>
#define BLACKVEL 1.0f/3.0f //3 veces por segundo
#define BLUEVEL 1.0f/6.0f //6 veces por segundo
#define REDVEL 1.0f/12.0f //3 veces por segundo
#define POINTSRED 1500
#define POINTSBLUE 1000
#define POINTSBLACK 500



//equivalencia de 0-100 a 0-90 , 66 equivale a 60 grados
#define HALF_BOUNCE_ANGLE 66
//medida que utilizamos para medir que animación es la actual
//equivalencia de 0-1 a 0-90 , 33 equivale a 30 grados
#define ANGLE_STEP 0.33f
//4 segundos
#define MAX_LIFE 4

#define NUM_FRAMES 3
#define NUM_FRAMES_DEAD 2

#define ANI_DURATION 4

#define DEAD_MOVY

#define N_EDGES 4



enum color { red ,blue, black };

enum animation {RIGHT, LEFT, TOPRIGHT, TOPLEFT, TOP, DEAD};		//Enum con los tipos de animaciones existentes

enum bounds{LEFT_BOUND, RIGHT_BOUND, TOP_BOUND, BOTTOM_BOUND};



class Duck
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

	float v = 0;
	
	float vAnim;

	float life = MAX_LIFE; //4 secs

	bool isDead = false;

	bool isFalling = false;

	float timerDead;

	float x = 0;
	float y = 0;

	float posDeadX = 0;
	float posDeadY = 0;
	
	float scale = 0;

	float movX = 0;
	float movY = 0;

	int frameAnimation = 0;			//contador de frames que lleva la animación

	int nextAnimation=0;			//variable que guarda el siguiente tipo de animación

	float timer = 0;			//timer para cambiar la animación cada CHANGEFRAME

	color c;

	int offset=0;

	Sprite** sprites;

	void kill();

	void collisionAnimationChange(bounds boundCollided);

	static inline float GetOtherAxisVel(float vel) {
		return 1 - (std::abs(vel));
	}
	static inline float GetRandomVel() {
		return ((std::rand() % (HALF_BOUNCE_ANGLE * 2 + 1)) - HALF_BOUNCE_ANGLE) / 100.f;
	}
	float getPosDeadX() {
		return posDeadX;
	}
	float getPosDeadY() {
		return posDeadY;
	}
	bool isFallingDuck() {
		return isFalling;
	}
public:
	Duck() {};

	Duck(float posX, float posY, float xMov, float yMov,color co, Sprite** _sprites, float scale_, const int(&colBounds)[N_EDGES]);

	void draw();

	bool update(float deltaTime);

	bool duckShoot(float pointerX, float pointerY);

	float getPosX() { return x; }
	float getPosY() { return y; }

	Vector2D getPos() { return Vector2D{x,y}; }

	float getWidth() { return  scale * SPRITE_WIDTH; }
	float getHeight() { return  scale * SPRITE_HEIGHT; }


	color getColor() { return (color)c; }

	
	

	void updateDuckAnimation();

	bool isKilled() { return isDead; }

	friend class DuckGroup;


};

