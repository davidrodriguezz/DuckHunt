#pragma once
#include <cstdint>
#include <string>


struct SDL_Window;
struct SDL_Renderer;



struct rgba {	
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};


class ImagePC;
struct ImageStructure;

class RendererPC
{
private:
	SDL_Window* window = nullptr;
	SDL_Renderer* render = nullptr; 

	int screen_width, screen_height;

	~RendererPC();
protected:
	RendererPC();
	
	static RendererPC* renderer;

	bool initPrivate(bool vsync, int width, int height, const std::string nameWindow);
	
public:
	static RendererPC* Instance();

	SDL_Renderer* getRenderer();

	void clear(uint32_t color);


	void present();

	static bool Init(bool vsync, int width, int height, const std::string nameWindow);

	void drawRect(int posX, int posY, int width, int height, uint32_t c);

	int getHeight() const;

	int getWidth() const;

	void ReleasePrivate();

	static void Release();

	rgba createRGBA(uint32_t baseColor);

	void putPixel(int x, int y, uint32_t baseColor);

	void putPixel(int x, int y, int r, int g, int b, int a);

	void drawImage(const ImagePC* img, int sx1, int sy1, int sx2, int sy2, int tx1, int ty1, int tx2, int ty2);

	ImagePC* createImage(ImageStructure* structure);
};
