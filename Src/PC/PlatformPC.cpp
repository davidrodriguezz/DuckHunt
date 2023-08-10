#include "PlatformPC.h"
#include <iostream>
#include <SDL.h>
#include <vector>

#include <cstring>
#include <iostream>
#include <fstream>

PlatformPC* PlatformPC::platform = nullptr;

PlatformPC::PlatformPC() { }

PlatformPC* PlatformPC::Instance()
{
	return platform;
}

PlatformPC::~PlatformPC() { }

//Inicializacion de SDL
bool PlatformPC::initPrivate() {
	
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::printf("error initializing SDL: %s\n", SDL_GetError());
		return false;
	}
	return true;
}


bool PlatformPC::Init()
{
	platform = new PlatformPC();
	bool ret = platform->initPrivate();
	if (!ret) {
		delete platform;
		platform = nullptr;
	}
	return ret;

}


void PlatformPC::Release()
{
	// cerrar SDL
	SDL_Quit();

	delete platform;
	platform = nullptr;
}

bool PlatformPC::tick()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
		{
			std::cout << "Cerrar aplicacion" << std::endl;
			return false;
		}
		else 
			Notify(event);

	}

	return true;
}

uint32_t PlatformPC::getTicks()
{
	return SDL_GetTicks();
}

FILE* PlatformPC::openFile(std::string fileName)
{
	FILE* inFile;
	const char* str = fileName.c_str();

	fopen_s(&inFile, str, "r");
	if (inFile == NULL)
	{
		std::cout << "No se puede abrir el archivo " << std::endl;
		return nullptr;
	}
	return inFile;
}



void PlatformPC::Suscribe(PCObserver* observer)
{
	list_observer_.push_back(observer);
}

void PlatformPC::UnSuscribe(PCObserver* observer)
{
	list_observer_.remove(observer);
}

void PlatformPC::Notify(SDL_Event event)
{
	std::list<PCObserver*>::iterator iterator = list_observer_.begin();


	while (iterator != list_observer_.end()) {
		(*iterator)->Update(event);
		++iterator;
	}
}
