#include "Sprite.h"
#include "Renderer.h"
#include "Image.h"
#include <iostream>
#include "RenderThread.h"


Sprite::Sprite(int x, int y, int w_, int h_,const Image* im)
{
	x1 = x; y1 = y; w = w_; h = h_; img = im;
}
//para los sprites que sean la imagen entera
Sprite::Sprite(const Image* im) {
	x1 = 0;
	y1 = 0;
	w = im->getWidth();
	h = im->getHeight();
	img = im;
}

void Sprite::Draw(int px1, int py1, float scaleX, float scaleY) //Dibujado del sprite que es enviado a la cola de comandos
{
	Command com;
	com.type = DRAW_IMAGE;
	com.drawImageInfo.img = img;
	com.drawImageInfo.x1 = x1; com.drawImageInfo.y1 = y1; 
	com.drawImageInfo.w = w; com.drawImageInfo.h = h;
	com.drawImageInfo.tx1 = px1; com.drawImageInfo.ty1 = py1; 
	com.drawImageInfo.tx2 = (int)px1 + (int)(w * scaleX); 
	com.drawImageInfo.ty2 = (int)py1 + (int)(h * (scaleY));

	RenderThread::Instance()->enqueueCommand(com);

}