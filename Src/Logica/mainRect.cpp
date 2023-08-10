#include <iostream>
#include <stdlib.h>
#include <vector>
#include <time.h>

//#include "checkML.h"

#include "Rectangle.h"
#include "Renderer.h"
#include "Platform.h"

#define VSYNC false

using namespace std;


const int N_RECTANGLES = 5;

vector<Rectangle> obj;
clock_t begin_time;
clock_t finish_time;

void init()
{
	
	for (int i = 0; i < N_RECTANGLES; i++)
	{
		int posX = rand() % (Renderer::Instance()->getWidth());
		int posY = Renderer::Instance()->getHeight() ;

		int c = rand() % 3;

		obj.push_back(Rectangle(posX, posY - 100, 150, 100, (color)c, Renderer::Instance()->getHeight(), Renderer::Instance()->getWidth()));
	}
}

void iniciarCronometro()
{
	begin_time = Platform::Instance()->getTicks();
}

void pararCronometro() {
	finish_time = Platform::Instance()->getTicks();
}

void update(float deltaTime)
{
	
	for (int i = 0; i < N_RECTANGLES; i++)
	{
		if (!obj[i].update(deltaTime)) //Rectangle out of window
		{
			int posX = rand() % (Renderer::Instance()->getWidth() - 150);
			int posY = Renderer::Instance()->getHeight();

			int c = rand() % 3;

			obj[i] = Rectangle(posX, posY-100, 150, 100, (color)c, Renderer::Instance()->getHeight(),Renderer::Instance()->getWidth());
		
		}
	}
	iniciarCronometro();
}

void draw()
{
	for (int i = 0; i < N_RECTANGLES; i++)
	{
		Renderer::Instance()->drawRect(obj[i].x, obj[i].y, obj[i].width, obj[i].height, obj[i].c);
	}
}

int mainrect()
{
	// Agrega checkML.h al proyecto e inclúyelo en todos los módulos del proyecto 
	// Escribe el siguiente comando al inicio de la función main
    //_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	srand(time(NULL));
	if (!Platform::Instance()->Init())
	{
		Platform::Instance()->release();
	}
	if (!Renderer::Instance()->Init(VSYNC))
	{
		Renderer::Instance()->release();
		Platform::Instance()->release();
	}
	init();
	
	uint32_t lastDraw = 0;
	float deltaTime = 0;

	int pendingFrames = 100000;
	iniciarCronometro();

	int fps = 0;
	float frametime = 0.f;
	
	while (Platform::Instance()->tick() && pendingFrames--)
	{
		lastDraw = Platform::Instance()->getTicks();

		Renderer::Instance()->clear(255, 255, 255);

		update(deltaTime);
		
		draw();
		
		Renderer::Instance()->present();

		deltaTime = ((float)((Platform::Instance()->getTicks() - lastDraw)) / 1000.f);
		fps++;
		frametime += deltaTime;
		if (frametime > 1.f) {
			cout << "FPS: " << fps / frametime << endl;
			fps = 0;
			frametime = 0;
		}

	}



	//Platform::Instance()->readFile("rgb.rgba",);

	//pararCronometro(); 
	

	
	Renderer::Instance()->release();
	Platform::Instance()->release();

    return 0;
}


int main() {

}

