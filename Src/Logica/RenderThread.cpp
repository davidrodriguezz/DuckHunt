#include "RenderThread.h"
#include "Renderer.h"

using namespace std;
RenderThread* RenderThread::RenderThread_ = nullptr;

RenderThread::RenderThread()
{
}

RenderThread::~RenderThread()
{
}

RenderThread* RenderThread::Instance()
{
	return RenderThread_;
}

void RenderThread::initPrivate() {
	stopThread = false;
	pendingFrames = 0;
	t = new thread(renderLoop);
}



void RenderThread::Init()
{
	RenderThread_ = new RenderThread();
	RenderThread_->initPrivate();
}

void RenderThread::ReleasePrivate() {
	
	stopThread = true; //acabar bucle
	t->join(); //ultima interaccion
	delete t;
	t = nullptr;

	
}

void RenderThread::Release()
{
	RenderThread_->ReleasePrivate();
	delete RenderThread_;
	RenderThread_ = nullptr;
}

void RenderThread::renderLoop() {
	RenderThread_->renderLoopInstance();
}

void RenderThread::renderLoopInstance()
{
	while (!stopThread)
	{
		Command command;
		if (queue.try_dequeue(command)) { //coger comando de la pila
			switch (command.type)
			{
			case DRAW_IMAGE:
			{
				DrawImageInfo info = command.drawImageInfo;
				Renderer::Instance()->drawImage(info.img, info.x1, info.y1, info.w, info.h, info.tx1, info.ty1, info.tx2, info.ty2);
				break;
			}
			case DRAW_TEXT:
			{
				DrawTextInfo info = command.textInfo;
				int number = info.points;
				int x = info.x;
				int y = info.y;
				int w = info.w;
				int h = info.h;

				// Convertir el número a una cadena de 6 dígitos con ceros a la izquierda
				char buffer[7]; // 6 dígitos + el terminador de cadena
				sprintf_s(buffer, "%06d", number);

				// Escribir los sprites de cada dígito uno a uno, de izquierda a derecha
				for (int i = 0; i < 6; i++) {
					Sprite digitSprite = (*info.sprites[buffer[i] - '0']);
					int digitX = x + (i * w); // Calcular la posición x del dígito en pantalla

					// Dibujar el sprite del dígito
					Renderer::Instance()->drawImage(digitSprite.getImg(), digitSprite.getX(), digitSprite.getY(), digitSprite.getW(), digitSprite.getH(), digitX, y, digitX+w, y+h);
				}

				break;
			}
			case CLEAR:
			{
				Renderer::Instance()->clear(command.clearInfo.color);
				break;
			}
			case PRESENT:
			{
				Renderer::Instance()->present();
				pendingFrames--;
				break;
			}
			default:
				break;
			}
		}
	}
}

void RenderThread::enqueueCommand(Command command)
{
	if (command.type == PRESENT) {
		pendingFrames++;
	}
	queue.enqueue(command);
}
