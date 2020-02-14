#include "PrickleBall.h"

PrickleBall::PrickleBall( Level& level )
	:
Entity( level )
{
	this->maxHealth = getMaxHealth();
	this->health = maxHealth;
	this->width = 16;
	this->height = 16;
}

PrickleBall::~PrickleBall()
{}

int PrickleBall::getMaxHealth()
{
	return 5;
}

void PrickleBall::onUpdate( float dt )
{
	Player& p = level.getPlayer();

	if( getPosX() + getWidth() > p.getPosX() &&
		getPosX() < p.getPosX() + p.getWidth() &&
		getPosY() + getWidth() > p.getPosY() &&
		getPosY() < p.getPosY() + p.getWidth() )
	{
		p.hurt( 10 );

		this->dead = true;
	}
}

void PrickleBall::render( Renderer& renderer )
{
	renderer.loadIdentity();

	renderer.translate( x, y, z );
	renderer.scale( 1.0, 1.0, 1.0 );
	renderer.rotate( 0, glm::vec3( 1, 1, 1 ) );

	renderer.drawSprite( width, height, 0, 0, glm::vec4( 255, 0, 0, 255 ) );
}