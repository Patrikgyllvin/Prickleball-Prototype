#include "Grenade.h"

Grenade::Grenade( float velX, float x, float y, Level& level )
	:
Entity( level ),
exploded( false ),
explosionCoolDown( 11 )
{
	this->width = 16;
	this->height = 16;

	this->x = x;
	this->y = y;
	this->velX = velX;

	this->maxHealth = 100;
	this->health = maxHealth;

	this->dead = false;
}

Grenade::~Grenade()
{}

void Grenade::onUpdate( float dt )
{
	if( this->explosionCoolDown > 0 && this->explosionCoolDown <= 10 )
		explosionCoolDown -= 10 * dt;
	else if( this->explosionCoolDown <= 0 )
	{
		this->health = 0;
		this->dead = true;
		this->exploded = false;
	}

	if( this->velX > 0 )
		this->velX -= 72 * dt;
	else if( this->velX <= 0 && !exploded )
	{
		std::vector<Entity*> ents = level.getEntitiesWithinAABB( glm::vec2( -120, 88 ), glm::vec2( 120, -88 ) );

		for( std::vector<Entity*>::iterator it = ents.begin(); it != ents.end(); ++it )
		{
			if( getPosX() + 96 / 2 > (*it)->getPosX() - (*it)->getWidth() &&
				getPosX() - 96 / 2 < (*it)->getPosX() + (*it)->getWidth() &&
				getPosY() + 96 / 2 > (*it)->getPosY() - (*it)->getHeight() &&
				getPosY() - 96 / 2 < (*it)->getPosY() + (*it)->getHeight() )
			{
				(*it)->hurt( 50 );
			}
		}
		
		exploded = true;
		explosionCoolDown = 10;
	}
}

void Grenade::render( Renderer& renderer )
{
	renderer.loadIdentity();

	renderer.translate( x, y, 0.0 );

	renderer.drawSprite( width, height, 64, 32, glm::vec4( 255, 255, 255, 255 ) );

	if( exploded )
		renderer.drawSprite( 96, 96, 96, 32, glm::vec4( 255, 255, 255, 255 ) );
}

bool Grenade::getExploded()
{
	return this->exploded;
}

float Grenade::getExplosionCoolDown()
{
	return this->explosionCoolDown;
}