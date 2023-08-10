#include "RendererPC.h"
#include <iostream>
#include <SDL.h>
#include <string>
#include "ImagePC.h"
#include "PlatformPC.h"
#include "Endianess.h"



using namespace std;


RendererPC* RendererPC::renderer = nullptr;

RendererPC::RendererPC() { }

bool RendererPC::initPrivate(bool vsync, int w_in, int h_in, const std::string nameWindow) {
	SDL_Window* window = SDL_CreateWindow(nameWindow.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w_in, h_in, NULL);
	if (window == nullptr)
	{
		this->window = nullptr;
		return false;
	}

	SDL_Renderer* render = SDL_CreateRenderer(window, -1, vsync ? SDL_RENDERER_PRESENTVSYNC : SDL_RENDERER_ACCELERATED); //Set vsync or not
	if (render == nullptr)
	{
		SDL_DestroyWindow(window);
		this->window = nullptr;
		this->render = nullptr;
		return false;
	}

	//SDL_WarpMouseInWindow(window, w_in / 2, h_in / 2);


	screen_width = w_in;
	screen_height = h_in;

	this->window = window;
	this->render = render;

	return true;
}


bool RendererPC::Init(bool vsync, int width, int height, const std::string nameWindow)
{

	renderer = new RendererPC();
	bool ret = renderer->initPrivate(vsync, width, height, nameWindow);
	if (!ret) {
		delete renderer;
		renderer = nullptr;
	}
	return ret;
}


RendererPC::~RendererPC() { }

RendererPC *RendererPC::Instance()
{
	return renderer;
}

SDL_Renderer* RendererPC::getRenderer() {
	return render;
}

void RendererPC::clear(uint32_t color)
{
	rgba col = createRGBA(color);
	SDL_SetRenderDrawColor(render, col.r, col.g, col.b, col.a);
	SDL_RenderClear(render);
}



void RendererPC::present()
{
	SDL_RenderPresent(render);
}



void RendererPC::drawRect(int posX, int posY, int width, int height, uint32_t color)
{
	SDL_Rect rect;
	rect.x = posX;
	rect.y = posY;
	rect.w = width;
	rect.h = height;

	rgba color_rgba = createRGBA(color);

	SDL_SetRenderDrawColor(render, color_rgba.r, color_rgba.g, color_rgba.b, color_rgba.a);
	SDL_RenderFillRect(render, &rect);
}


int RendererPC::getHeight() const
{
	return screen_height;
}

int RendererPC::getWidth() const
{
	return screen_width;
}

void RendererPC::ReleasePrivate() {
	
	// destroy renderer
	SDL_DestroyRenderer(render);
	render = nullptr;
	
	// destroy window	
	SDL_DestroyWindow(window);
	window = nullptr;
	
}

void RendererPC::Release()
{
	
	renderer->ReleasePrivate();

	delete renderer;
	renderer = nullptr;
}

rgba RendererPC::createRGBA(uint32_t baseColor) {
	uint8_t r = (baseColor >> 24) & 0xFF;
	uint8_t g = (baseColor >> 16) & 0xFF;
	uint8_t b = (baseColor >> 8) & 0xFF;
	uint8_t a = (baseColor >> 0) & 0xFF;
	return { r,g,b,a };

}


void RendererPC::putPixel(int x, int y, uint32_t baseColor)
{
	rgba color = createRGBA(baseColor);
	SDL_SetRenderDrawColor(renderer->render, color.r, color.g, color.b, color.a);
	SDL_RenderDrawPoint(renderer->render, x, y);
}

void RendererPC::putPixel(int x, int y, int r , int g , int b , int a)
{	
	if (a != 255) return;
	SDL_SetRenderDrawColor(renderer->render, r, g, b, a);
	SDL_RenderDrawPoint(renderer->render, x, y);
}



void RendererPC::drawImage(const ImagePC* img, int sx1, int sy1, int sx2, int sy2, int tx1, int ty1, int tx2, int ty2)
{
	SDL_Rect srcRect{sx1,sy1, (sx2), (sy2)};
	SDL_Rect destRect{ tx1,ty1, (tx2-tx1 )+1, (ty2-ty1 )+1 };
	SDL_RenderCopy(renderer->render, img->getTexture(), &srcRect, &destRect);
}

ImagePC* RendererPC::createImage(ImageStructure* structure)
{
	ImagePC* imagen = new ImagePC();

	if (!imagen->assembleImage(structure)) //Error rellenando la imagen
	{
		delete imagen;
		return nullptr;
	}

	return imagen;
}
