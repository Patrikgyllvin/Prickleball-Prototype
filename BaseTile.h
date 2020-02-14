#ifndef BASETILE_H
#define BASETILE_H

#include <glm/glm.hpp>

#include "Tile.h"

class BaseTile : public Tile
{
public:
    BaseTile( int texW, int texH, int texPosX, int texPosY, bool solid, int x, int y );
    virtual ~BaseTile();

	void tick( float dt );
	void render( Renderer& renderer );

private:
};

#endif