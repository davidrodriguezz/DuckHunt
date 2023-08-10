#pragma once
#include "Observer.h"
#include <SDL_events.h>
#include <queue>

#define DEAD_ZONE 6000
#define MAX_VALUE_AXIS 32767.f

#define keyCode  event.key.keysym.sym
#define buttonCode event.cbutton.button

class Controller;

class InputPC : public IObserver<SDL_Event> 
{
private:
	~InputPC();
	
	std::queue<SDL_Event> eventQueue;
	SDL_GameController* controller;

	bool initPrivate(bool relative_mouse);
	void releasePrivate();

	//Comprobar si la tecla se esta manteninedo presionada o no
	bool circlePressed = false;
	bool crossPressed = false;
	bool squarePressed = false;
protected:

	InputPC();

	static InputPC* input;
public:
	

	static InputPC* Instance();



	static bool Init(bool relative_mouse);

	static void Release();

	Controller getInput();

	void Update(const SDL_Event)override;
};

