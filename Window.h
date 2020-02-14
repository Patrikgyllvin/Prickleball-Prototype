#ifndef WINDOW_H
#define WINDOW_H
#include <GL/GLEW.h>
#include <SDL2/SDL.h>

#include <SDL2/SDL_image.h>

#include <SDL2/SDL_opengl.h>

#include <iostream>

#include <cstdio>
#include <stdlib.h>

#include <ctime>

#include "InputHandler.h"

#include "Game.h"

#include "Renderer.h"

#include <sstream>

//#include <vld.h>

class Window
{
public:
	Window();
	~Window();

	int setupWindow();
	int setupGL();

	void run();
	
private:
	SDL_Window* window;

	SDL_GLContext glContext;

	InputHandler* inputHandler;
	Game* game;
	Renderer* renderer;

	bool running, paused;

	int fps;
	long lastFrame;

	int w, h;

	bool OGL3;

	double getDelta();

	void setFPS( int fps );

	void load();

	void tick( float delta );
	void render();

	void processEvents( SDL_Event e );
};

#endif
