#include "ImagePC.h"

#include "RendererPC.h"
#include "Endianess.h"
#include "ImageStructure.h"
#include <vector>

#include <SDL.h>

#ifdef PLATFORM_IS_LITTLE_ENDIAN     
#define PIXEL_FORMAT SDL_PIXELFORMAT_ABGR32
#elif
#define PIXEL_FORMAT SDL_PIXELFORMAT_RGBA32
#endif


ImagePC::~ImagePC()
{
	SDL_DestroyTexture(texture);
}

bool ImagePC::assembleImage(ImageStructure* structure)
{
	int c = 0;

	width = structure->width;
	height = structure->height;

	//creacion de textura
	surface = SDL_CreateRGBSurfaceWithFormatFrom(structure->pixeles, width, height, 32, 4 * width, PIXEL_FORMAT);
	if (surface == NULL)
	{
		return false;
	}
	texture = SDL_CreateTextureFromSurface(RendererPC::Instance()->getRenderer(), surface);
	SDL_FreeSurface(surface);
	if (texture == NULL)
	{
		return false;
	}

	return true;
}


