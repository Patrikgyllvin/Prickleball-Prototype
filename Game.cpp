#include "Game.h"

Game::Game()
	:
cam( new Camera( glm::vec3( 0, 0, 160 ) ) ),
level( new Level() ),
player( new Player( getCamera(), *level ) )
{}

Game::~Game()
{
	delete cam;
}

void Game::load( Renderer* renderer )
{
	this->renderer = renderer;

	level->load( this->renderer );
	level->loadLevel( this->player );

	player->setPosX( -104 );
	player->setPosY( 0 );
	player->setPosZ( 0 );
}

void Game::tick( float delta )
{
	level->tick( delta );
	
	player->tick( delta );
}

void Game::render()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glClearColor( 0.0, 0.0, 0.0, 1.0 );

	level->render();

	player->render( *renderer );
}

Camera& Game::getCamera()
{
	return *this->cam;
}