#include "BaseTile.h"

BaseTile::BaseTile( int texW, int texH, int texPosX, int texPosY, bool solid, int x, int y )
:
Tile( texW, texH, texPosX, texPosY, solid, x, y )
{}

BaseTile::~BaseTile()
{}

void BaseTile::tick( float dt )
{}

void BaseTile::render( Renderer& renderer )
{
	if( this->getSolid() )
	{
		renderer.loadIdentity();

		renderer.translate( this->x, this->y, 0 );
		renderer.scale( 1.0, 1.0, 1.0 );
		renderer.rotate( 0, glm::vec3( 1, 1, 1 ) );

		renderer.drawSprite( texW, texH, texPosX, texPosY, glm::vec4( 255, 255, 255, 255 ) );
	}
}