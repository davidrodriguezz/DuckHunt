#pragma once

#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

#include "Controller.h"
#include <SDL_events.h>
#include "Observer.h"

using PCObserver = IObserver<SDL_Event>;

class PlatformPC : private ISubject<SDL_Event>
{
private:
	~PlatformPC();
	bool initPrivate();

protected:

	PlatformPC();

	static PlatformPC* platform;

public:
	static PlatformPC* Instance();

	static bool Init();

	static void Release();

	bool tick();

	uint32_t getTicks();

	FILE* openFile(std::string filename);

	//ISubject
	void Suscribe(PCObserver* observer) override;
	void UnSuscribe(PCObserver* observer) override;
	void Notify(SDL_Event evento) override;
};

