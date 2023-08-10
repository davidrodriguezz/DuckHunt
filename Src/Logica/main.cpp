
#include "screen_sprite_defines.h"

#include "Image.h"
#include "Renderer.h"
#include "Platform.h"

#include "RenderThread.h"

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <time.h>

#include "ImageRelated.h"
#include "DuckGroup.h"

#include "Pointer.h"
#include "Foreground.h"

#include "Input.h"
#include "Controller.h"

#include "ImageStructure.h"

#include "checkML.h"

using namespace std;

#define VSYNC true

//RGBA
#define background_color 0x3cbcfcff;

#define N_DUCKS 5

#define N_FORE 3

#define POINTER 18

#define WIDTH_TILES 40

#define RELATIVE_CURSOR true

#define IMAGES_PER_DUCK 5

#define GROUND 2

#define N_POINTS 3
#define NUMBERS 10


void deleteImages(int LastImageLoaded, Image* (&imagenes)[NUM_IMAGES]) {

	for (int x = 0; x < LastImageLoaded; x++)
		delete imagenes[x];

	std::cout << "Error carga de imagenes " << std::endl;
}
/**
* Carga de todas las imagenes del juego
* En caso de error, borra las imagenes cargadas hasta el momento y devuelve false
*/

bool initImgs(const string(&pathsToFile)[NUM_IMAGES], Image* (&imagenes)[NUM_IMAGES]) {

	int i = 0;
	//carga de imagenes
	for (string path : pathsToFile) {
		FILE* file = Platform::Instance()->openFile(path);
		if (file == nullptr) {
			std::cout << "Error apertura de archivo, revise la ruta y si el archivo existe: " << path << std::endl;
			deleteImages(i, imagenes);
			return false;
		}
		ImageStructure* actualImageStruct = ImageRelated::readFile(file);
		if (actualImageStruct == nullptr) //Error leyendo archivo
		{
			std::cout << "Error lectura de archivo imagen: " << path << std::endl;
			fclose(file);
			deleteImages(i, imagenes);
			return false;
		}
		else if (fclose(file) == EOF) //Error cerrando archivo
		{
			std::cout << "Error cerrar archivo imagen: " << path << std::endl;
			delete actualImageStruct;
			deleteImages(i, imagenes);
			return false;
		}
		else
		{
			imagenes[i] = Renderer::Instance()->createImage(actualImageStruct);
			if (imagenes[i] == nullptr)
			{
				std::cout << "Error creando imagen: " << path << std::endl;
				delete actualImageStruct;
				deleteImages(i, imagenes);
				return false;
			}
			
			i++;
			//delete de la estructura utilizada para leer el fichero y crear la imagen
			delete actualImageStruct;
		}
	}

	std::cout << "Fin de carga de imagenes " << std::endl;

	return true;
}

/**
*Crea los sprites que se guardan de la siguiente forma: primero recorremos en orden las imagenes de cada uno de los colores (ej. azul0, azul45, azul90)
* y vamos guardando en el vector de sprites cada uno de los sprites en orden de izquierda derecha y arriba abajo
*carga de los 15 sprites
*/
void spriteCreator(Image* (&imagenes)[NUM_IMAGES], Sprite* (&sprites)[NUM_SPRITES_DUCK], int group) {
	/**
	* dimensiones en tiles de cada tipo de imagen en el orden:
	* color_0, color_45, color_90,color_-90, color_dado
	* dado en formato {columnas, filas}
	*/
	const std::pair<int, int> dimensiones[IMAGES_PER_DUCK]{ {3,2}, {3,2}, {3,1}, {2,1}, {1,1} };

	//offset para que las imagenes se escriban en el array lineal sprites 
	//si la primera imagen tiene 6 sprites queremos que la segunda empiece en la posicion 7
	int offset = 0;
	// por legibilidad, ancho y alto de la imagen en tiles
	int image_width, image_height;
	//ancho y alto en pixeles
	int sprite_width, sprite_height;
	// va desde mi primera imagen hasta mi primera imagen+IMAGES_PER_DUCK, de ahi el 
	// (group + 1) * IMAGES_PER_DUCK
	for (int index_image = group * IMAGES_PER_DUCK; index_image < (group + 1) * IMAGES_PER_DUCK; index_image++) {
		int local_image = index_image % IMAGES_PER_DUCK;
		Image* image = imagenes[index_image];
		
		image_height = dimensiones[local_image].second;
		image_width = dimensiones[local_image].first;
		
		sprite_width = image->getWidth() / image_width;
		sprite_height = image->getHeight() / image_height;

		//ver offset
		if (index_image != group * IMAGES_PER_DUCK)
				offset += dimensiones[local_image - 1].first * dimensiones[local_image - 1].second;

		for (int y = 0; y < image_height; y++) 
			for (int x = 0; x < image_width; x++) 
				sprites[(y * image_width + x)+ offset] = new Sprite(sprite_width*x, sprite_height *y, sprite_width, sprite_height, image);
			
	}
}


void pointerCreator(Image* imagen, Pointer* &pointer, float scale) {
	// se toma la imagen completa
	Sprite* pointerSprite = new Sprite(imagen);

	int posX = Renderer::Instance()->getWidth() / 2;
	int posY = Renderer::Instance()->getHeight() / 2;

	pointer = new Pointer(Vector2D{ posX, posY },pointerSprite,scale);
}


void foregroundCreator(Image* (&imagenes)[NUM_IMAGES], Sprite* (&sprites)[N_FORE], Foreground* (&foreground)[N_FORE]) {
	// calculamos cuantos pixeles reales hay por tile requerido
	// 40 numero total de tiles de largo
	int screenWidth = Renderer::Instance()->getWidth();
	int screenHeight = Renderer::Instance()->getHeight();

	float pixelsPerTile = (float)Renderer::Instance()->getWidth() / (float)WIDTH_TILES; 

	const foregroundInfo info[N_FORE]{
	foregroundInfo(Vector2D{ screenWidth * 4.0f / 40.0f, screenHeight * 0.5f / 22.5f },10,14),
	foregroundInfo(Vector2D{ screenWidth * 28.0f / 40.0f, screenHeight * 11.5f / 22.5f },4,3),
	foregroundInfo(Vector2D{ 0, screenHeight * 14.5f / 22.5f },40,8)
	};

	
	int y = 0;
	/*
	* El foreground se carga inmediatamente despues de los patos, tanto en ejecucion como en rutas, de ahi el x = NUM_IMAGES_DUCK
	*/
	for (int x = NUM_IMAGES_DUCK; x < NUM_IMAGES - 3; x++, y++) {
		Image* img = imagenes[x];
		// se toma la imagen completa
		sprites[y] = new Sprite(img);
		foreground[y] = new Foreground(info[y].pos,sprites[y], pixelsPerTile, info[y].sizeX, info[y].sizeY);
	}
	
}

void duckPointsCreator(Image * imagen, Sprite* (&sprites)[N_POINTS]) {


	const std::pair<int, int> dimensiones{ 2,4 };
	int y = 0;

	for (int x = (dimensiones.first * dimensiones.second)-1; x >= 0 ; x--) {
		if (x == 0) {
			sprites[y] = new Sprite((int)(x % 2 * imagen->getWidth()/2), (int)(x / 2 * imagen->getHeight()/4), imagen->getWidth() / 2, imagen->getHeight() / 4, imagen);
			y++;

		}
		else if (x == 2) {
			sprites[y] = new Sprite((int)(x % 2 * imagen->getWidth()/2), (int)(x/2*imagen->getHeight()/4), imagen->getWidth() / 2, imagen->getHeight() / 4, imagen);
			y++;
		}
		else if (x == 3) {
			sprites[y] = new Sprite((int)(x % 2 * imagen->getWidth()/2), (int)(x / 2 * imagen->getHeight()/4), imagen->getWidth() / 2, imagen->getHeight() / 4, imagen);
			y++;
		}
	}

}

void pointsCreator(Image* imagen, Sprite* (&sprites)[NUMBERS]) {


	const std::pair<int, int> dimensiones{ 1,10 };
	int y = 0;

	for (int x = 0; x < (dimensiones.first * dimensiones.second); x++) {
			sprites[y] = new Sprite((int)(x % NUMBERS * imagen->getWidth() / NUMBERS), 0, imagen->getWidth() / NUMBERS, imagen->getHeight(), imagen);
			y++;
	}

}

void handleEvents(DuckGroup* ducks, Pointer* pointer)
{
	Controller controller = Input::Instance()->getInput();

	pointer->handleEvents(controller);

	ducks->handleEvents(controller, pointer->getMidPositionX(), pointer->getMidPositionY());
}


int main() {
	srand(time(NULL));
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	const string assets = "assets/";

	const string pathsToFile[NUM_IMAGES] = { assets + "rojo_0.rgba", assets + "rojo_45.rgba" , assets + "rojo_90.rgba", assets + "rojo_-90.rgba", assets + "rojo_dado.rgba",
											assets + "azul_0.rgba", assets + "azul_45.rgba" , assets + "azul_90.rgba", assets + "azul_-90.rgba", assets + "azul_dado.rgba",											
											assets + "negro_0.rgba", assets + "negro_45.rgba" , assets + "negro_90.rgba", assets + "negro_-90.rgba", assets + "negro_dado.rgba",
											assets + "arbol.rgba", assets + "arbusto.rgba" , assets + "suelo.rgba", assets + "cursor.rgba", assets+ "puntos.rgba", assets+"digitosVerdes.rgba"};
	
	Sprite* blueSprites[NUM_SPRITES_DUCK];
	Sprite* redSprites[NUM_SPRITES_DUCK];
	Sprite* blackSprites[NUM_SPRITES_DUCK];

	Sprite* foregroundSprites[N_FORE];
	Sprite* pointsDuckSprites[N_POINTS];
	Sprite* pointSprites[NUMBERS];
	Foreground* foreground[N_FORE];

	Image* imagenes[NUM_IMAGES];

	Pointer* pointer = nullptr;



	//inicializacion de singletons
	//en caso de fallar alguna inicializacion, la constructora se encarga de borrar / desinicializar todo y devuelve false
	if (!Platform::Instance()->Init()) {
		std::cout << "Fallo al inicializar Platform" << std::endl;
		return -1;
	}

	if (!Renderer::Instance()->Init(VSYNC, SCREEN_WIDTH, SCREEN_HEIGHT, "Practica 4"))
	{
		std::cout << "Fallo al inicializar Renderer" << std::endl;
		Platform::Instance()->Release();
		return -1;
	}
	if (!Input::Instance()->Init(RELATIVE_CURSOR))
	{
		std::cout << "Fallo al inicializar Input" << std::endl;
		Renderer::Instance()->Release();
		Platform::Instance()->Release();

		return -1;
	}

	float scale = Renderer::Instance()->getWidth() / 10.f / SPRITE_WIDTH;

	bool ret = initImgs(pathsToFile, imagenes);
	if (!ret) //Error en la carga de las imagenes
	{
		Input::Instance()->Release();
		Renderer::Instance()->Release();
		Platform::Instance()->Release();
		return -1;
	}
	//creacion de sprites de patos
	spriteCreator(imagenes, redSprites, red);
	spriteCreator(imagenes, blueSprites, blue);
	spriteCreator(imagenes, blackSprites, black);
	duckPointsCreator(imagenes[19], pointsDuckSprites);
	pointsCreator(imagenes[20], pointSprites);
	//creacion de objetos en escena(foreground, patos y cross-hair)------------------------------
	foregroundCreator(imagenes, foregroundSprites, foreground);

	//dependencia de foreground por parte de los patos, necesito que se haya creado el suelo para poder ponerles los bounds de colision bien
	int bounds[N_EDGES] = { 0,Renderer::Instance()->getWidth(), 0, Renderer::Instance()->getHeight() - (int)(foreground[GROUND]->getHeight() *scale) };
	DuckGroup ducks(N_DUCKS, blueSprites, redSprites, blackSprites, pointsDuckSprites, pointSprites, bounds);
	ducks.createDucks();

	pointerCreator(imagenes[POINTER], pointer, scale);


	//TODO INICIALIZADO
	RenderThread::Instance()->Init();

	//comando clear
	Command clear;
	clear.type = CLEAR;
	clear.clearInfo.color = background_color;

	//comando present
	Command present;
	present.type = PRESENT;


	//variables para el bucle / conteo de fps
	uint32_t lastDraw = 0;
	float deltaTime = 0;
	int pendingFrames = 1000000000;
	int fps = 0;
	float frametime = 0.f;

	std::cout << "Inicio bucle " << std::endl;
	while (Platform::Instance()->tick() && pendingFrames--)
	{
		lastDraw = Platform::Instance()->getTicks();

		//------CLEAR-----------		
		RenderThread::Instance()->enqueueCommand(clear);
		//---------UPDATE---------

		ducks.update(deltaTime);

		pointer->update(deltaTime);

		handleEvents(&ducks, pointer);

		//-------DRAW-------------


		for (Foreground* f : foreground) {
			f->draw();
		}
				ducks.draw();
		pointer->draw();

		//------PRESENT---------

		RenderThread::Instance()->enqueueCommand(present);

		//Espera activa para evitar que la logica se adelante al render demasiado
		while (RenderThread::Instance()->getPendingFrames() > 2) {}


		//---------delta time y calculo de fps
		deltaTime = ((float)((Platform::Instance()->getTicks() - lastDraw)) / 1000.f);
		fps++;
		frametime += deltaTime;
		if (frametime > 1.f) {
			std::cout << "FPS: " << fps / frametime << std::endl;
			fps = 0;
			frametime = 0;
		}
	}

	//------liberacion de memoria

	RenderThread::Instance()->Release();

	//se encarga de borrar su sprite
	delete pointer;

	for (Foreground* fore : foreground)
		delete fore;
	for (Sprite* spr : blueSprites)
		delete spr;
	for (Sprite* spr : redSprites)
		delete spr;
	for (Sprite* spr : blackSprites)
		delete spr;
	for (Sprite* spr : foregroundSprites)
		delete spr;	
	for (Image* img : imagenes)
		delete img;
	

	
	Input::Instance()->Release();
	Renderer::Instance()->Release();
	Platform::Instance()->Release();
	
	
	return 0;
}