#ifndef GAME_H
#define GAME_H

#include <iostream>

#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#include "Camera.h"

#include "InputHandler.h"

#include "Renderer.h"

#include "Level.h"

#include "Player.h"

class Game
{
public:
	Game();
	~Game();
	
	void load( Renderer* renderer );

	void tick( float delta );
	void render();

	Camera& getCamera();

private:
	Camera* cam;
	Renderer* renderer;

	Level* level;

	Player* player;
};

#endif