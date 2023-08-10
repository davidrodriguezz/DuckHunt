#pragma once
#include <iostream>
#include <thread>
#include "concurrentqueue.h"
#include "atomicops.h"
#include "Renderer.h"
#include "Image.h"
#include "Command.h"
//#include "checkML.h"



class RenderThread
{
private:
	static RenderThread* RenderThread_;

	std::thread* t;
	moodycamel::ConcurrentQueue<Command> queue;//pila de comandos
	volatile std::atomic<int> pendingFrames;
	volatile std::atomic<bool> stopThread;

	void initPrivate();
	void ReleasePrivate();

	//loop que realizará la hebra
	static void renderLoop();
	void renderLoopInstance();
	

public:

	RenderThread();
	~RenderThread();

	static RenderThread* Instance();

	static void Init();
	static void Release();


	void enqueueCommand(Command command);

	int getPendingFrames() { return pendingFrames; }
};

