#include "Tile.h"

Tile::Tile( int texW, int texH, int texPosX, int texPosY, bool solid, int x, int y )
:
x( x ),
y( y ),
texPosX( texPosX ),
texPosY( texPosY ),
texW( texW ),
texH( texH ),
solid( solid )
{}

Tile::~Tile()
{}

bool Tile::getSolid()
{
	return this->solid;
}

void Tile::setX( float x )
{
	this->x = x;
}

float Tile::getX()
{
	return this->x;
}

void Tile::render( Renderer& renderer )
{}