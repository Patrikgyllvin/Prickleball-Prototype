#include "Bullet.h"

Bullet::Bullet( float x, float y, float velX, float velY, Level& level )
	:
Entity( x, y, 0.0, velX, velY, 0.0, 0.0, 0.0, 0.0, level )
{
	this->maxHealth = this->getMaxHealth();
	this->health = this->maxHealth;

	this->width = 6.4;
	this->height = 3.2;
}

Bullet::Bullet( float x, float y, float z, float velX, float velY, float velZ, float rotX, float rotY, float rotZ, Level& level )
	:
Entity( x, y, z, velX, velY, velZ, rotX, rotY, rotZ, level )
{
	this->maxHealth = this->getMaxHealth();
	this->health = this->maxHealth;

	this->width = 6.4;
	this->height = 3.2;
}

Bullet::~Bullet()
{}

void Bullet::onUpdate( float dt )
{
	if( ticks > 3 )
		this->dead = true;

	std::vector<Entity*> ents = level.getEntitiesWithinAABB( glm::vec2( -120, 88 ), glm::vec2( 120, -88 ) );

	for( std::vector<Entity*>::iterator it = ents.begin(); it != ents.end(); ++it )
	{
		float x = (*it)->getPosX();
		float y = (*it)->getPosY();
		float w = (*it)->getWidth();
		float h = (*it)->getHeight();

		if( getPosX() + getWidth() < x + w )
			if( getPosX() - getWidth() > x - w )
				if( getPosY() + getHeight() < y + h )
					if( getPosY() - getHeight() > y - h )
					{
						(*it)->hurt( 5 );
						this->dead = true;
					}
	}
}

void Bullet::render( Renderer& renderer )
{
	renderer.loadIdentity();

	renderer.translate( this->x, this->y, 0.0 );
	renderer.scale( width, height, 1 );
	renderer.rotate( 0, glm::vec3( 1, 1, 1 ) );

	renderer.drawQuad( glm::vec4( 0, 0, 0, 255 ) );
}