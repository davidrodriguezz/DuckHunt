#pragma once

#include <vector>


struct SDL_Surface;
struct SDL_Texture;

struct ImageStructure;

class ImagePC {
private:
	

public:
	ImagePC() {};
	~ImagePC();
	
	bool assembleImage(ImageStructure* structure);

	 SDL_Texture* getTexture() const { return texture; };
	
	int getWidth() const { return width; };
	int getHeight() const { return height; };
	

protected:
	int width = 0;
	int height = 0;

	SDL_Surface* surface = nullptr;
	SDL_Texture* texture = nullptr;
};