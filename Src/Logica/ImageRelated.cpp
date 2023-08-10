#include "ImageRelated.h"
#include "Endianess.h"
#include "ImageStructure.h"
#include <iostream>
#include <cstdint>
#include <iomanip>


using namespace std;


//se reciben los datos de imagen con la siguiente estructura
// width y height como 4 bytes en formato big endian
//los pixeles de la imagen de manera consecutiva en formato RGBA
//devuelve los pixeles en formato RGBA
ImageStructure* ImageRelated::readFile(FILE* inFile)
{
	ImageStructure* imgStruct = new ImageStructure(); //main.cpp se encarga de borrarlo si no falla el metodo

	int ancho, alto;
	ancho = alto = 0;

	size_t err = 0;
	if ((err = fread(&ancho, sizeof(int), 1, inFile)) != 1)
	{
		delete imgStruct;
		return nullptr;
	}

	if ((err = fread(&alto, sizeof(int), 1, inFile)) != 1)
	{
		delete imgStruct;
		return nullptr;
	}


#ifdef PLATFORM_IS_LITTLE_ENDIAN
		ancho = SWAP_ENDIANESS_32(ancho);
		alto = SWAP_ENDIANESS_32(alto);
#endif
		
	size_t imgSize = ancho * alto;

	imgStruct->width = ancho;
	imgStruct->height = alto;
	

	//pixel aux;
	
	uint32_t* buffer = new uint32_t[imgSize]; // Se borra en el propio metodo
	
	if ((err = fread(buffer, sizeof(uint32_t), imgSize, inFile)) != imgSize)
	{
		delete []buffer;
		delete imgStruct;
		return nullptr;
	}

	imgStruct->pixeles = new uint32_t[ancho * alto]; // main.cpp lo borra junto a ImageStructure
	
#ifdef PLATFORM_IS_LITTLE_ENDIAN
	for (int pixel = 0; pixel < imgSize; ++pixel) 
		imgStruct->pixeles[pixel] = SWAP_ENDIANESS_32(buffer[pixel]);
#endif

	delete [] buffer;

	return imgStruct;

	
}
