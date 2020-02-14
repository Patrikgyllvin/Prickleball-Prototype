#ifndef TILE_H
#define TILE_H

#include "Renderer.h"

class Tile
{
public:
    Tile( int texW, int texH, int texPosX, int texPosY, bool solid, int x, int y );
    virtual ~Tile();

    virtual void tick( float dt ) = 0;
	virtual void render( Renderer& renderer );

	bool getSolid();

	void setX( float x );

	float getX();

protected:
    float x, y, texPosX, texPosY, texW, texH;

    bool solid;
};

#endif // TILE_H
