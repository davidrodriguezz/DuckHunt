#include "InputPC.h"
#include "Controller.h"
#include "PlatformPC.h"
#include <SDL_events.h>
#include <iostream>

using namespace std;


InputPC* InputPC::input = nullptr;

using PCObserver = IObserver<SDL_Event>;

InputPC::InputPC()
{

}

InputPC* InputPC::Instance()
{
	return input;
}

bool InputPC::initPrivate(bool relative_mouse) {


	// Recorremos el num de Joysticks
	for (int i = 0; i < SDL_NumJoysticks(); i++)
	{
		if (SDL_IsGameController(i)) //Comprobamos que es un mando
		{
			input->controller = SDL_GameControllerOpen(i); //Cogemos el primer mando que encuentre
			break;
		}
	}

	if (relative_mouse)
		SDL_SetRelativeMouseMode(SDL_bool(relative_mouse));

	PlatformPC::Instance()->Suscribe(this);

	return true;

}

bool InputPC::Init(bool relative_mouse)
{
	input = new InputPC();
	bool ret = input->initPrivate(relative_mouse);
	if (!ret) {
		delete input;
		input = nullptr;
	}

	return ret;
}

Controller InputPC::getInput()
{
	Controller out;
	SDL_Event event;

	while (!eventQueue.empty())
	{
		event = eventQueue.front();


		if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_RCTRL)
			{
				circlePressed = true;
				crossPressed = true;
			}
			else if (event.key.keysym.sym == SDLK_RSHIFT)
			{
				circlePressed = true;
				crossPressed = true;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT)
			{
				out.leftStickX = 1;
			}
			else if (event.key.keysym.sym == SDLK_LEFT)
			{
				out.leftStickX = -1;
			}
			else if (event.key.keysym.sym == SDLK_UP)
			{
				out.leftStickY = -1;
			}
			else if (event.key.keysym.sym == SDLK_DOWN)
			{
				out.leftStickY = 1;
			}
			else if (event.key.keysym.sym == SDLK_0) {
				squarePressed = true;
			}
		}
		else if (event.type == SDL_KEYUP) //Comprobamos que las teclas se han dejado de presionar para reiniciar el input
		{
			if (event.key.keysym.sym == SDLK_RCTRL)
			{
				circlePressed = false;
				crossPressed = false;
			}
			else if (event.key.keysym.sym == SDLK_RSHIFT)
			{
				circlePressed = false;
				crossPressed = false;
			}
			else if (event.key.keysym.sym == SDLK_0) {
				squarePressed = false;
			}
		}
		else if (event.type == SDL_MOUSEMOTION)
		{
			out.gyroscopeAngularVelocityX += event.motion.xrel;
			out.gyroscopeAngularVelocityY += event.motion.yrel;
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			out.circle = true;
			out.cross = true;
		}
		else if (event.type == SDL_CONTROLLERBUTTONDOWN)
		{
			if (buttonCode == SDL_CONTROLLER_BUTTON_A)
				out.cross = true;
			else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
				out.circle = true;
		}
		if (event.type == SDL_JOYDEVICEADDED) {
			if (!input->controller) {
				input->controller = SDL_GameControllerOpen(0);
			}
		}
		if (event.type == SDL_JOYDEVICEREMOVED) {
			SDL_GameControllerClose(input->controller);
			input->controller = nullptr;

			//se busca un nuevo mando en caso de que se haya desconectado uno
			for (int i = 0; i < SDL_NumJoysticks(); i++)
			{
				if (SDL_IsGameController(i)) //Comprobamos que es un mando
				{
					input->controller = SDL_GameControllerOpen(i); //Cogemos el primer mando que encuentre
					break;
				}
			}

		}
		eventQueue.pop();
	}

	//Se cambia el valor en caso de que se haya pulsado alguna tecla
	//Para evitar que pise el input del teclado en caso de haber
	if(circlePressed) out.circle = circlePressed;
	if (crossPressed) out.cross = crossPressed;
	if (squarePressed) out.square = squarePressed;

	//Control de axis de mando
	if (input->controller != nullptr)
	{
		//--------------Joystick izquierdo------------------------------
		auto leftX = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
		auto leftY = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);

		if ((leftX < -DEAD_ZONE) || (leftX > DEAD_ZONE)) // 6000 deadzone
		{
			out.leftStickX = leftX / MAX_VALUE_AXIS;
		}
		if ((leftY < -DEAD_ZONE) || (leftY > DEAD_ZONE)) // 6000 deadzone
		{
			out.leftStickY = leftY / MAX_VALUE_AXIS;
		}

		//----------------------L2 y R2------------------------------

		auto L2 = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
		auto R2 = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

		if ((L2 < -DEAD_ZONE) || (L2 > DEAD_ZONE)) // 6000 deadzone
		{
			out.L2 = L2 / MAX_VALUE_AXIS;
		}
		if ((R2 < -DEAD_ZONE) || (R2 > DEAD_ZONE)) // 6000 deadzone
		{
			out.R2 = R2 / MAX_VALUE_AXIS;
		}
	}
	return out;
}

void InputPC::Update(const SDL_Event ev)
{
	eventQueue.push(ev);
}

InputPC::~InputPC()
{

}

void InputPC::releasePrivate() {
	SDL_GameControllerClose(controller); //Comprobar que no hay q hacer mas
	controller = nullptr;
}

void InputPC::Release()
{
	input->releasePrivate();

	delete input;
	input = nullptr;
}