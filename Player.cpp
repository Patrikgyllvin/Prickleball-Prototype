#include "Player.h"

Player::Player( Camera& cam, Level& level )
	:
Entity( level ),
cam( cam ),
shootCoolDown( 0 ),
hurtCoolDown( 0 ),
score( 0 ),
goal( 100000 ),
grenadeCoolDown( 0 ),
notEnoughScores( false ),
startTime( 0 )
{
	this->maxHealth = getMaxHealth();
	this->health = maxHealth;
	this->width = 16;
	this->height = 32;

	this->grenade = NULL;

	heartCoolDown[ 0 ] = 2;
	heartCoolDown[ 1 ] = 2;
	heartCoolDown[ 2 ] = 2;
}

Player::~Player()
{}
#include <iostream>
void Player::onUpdate( float dt )
{
	if( InputHandler::getKey( SDLK_a ) )
		this->velY = 64;
	else if ( InputHandler::getKey( SDLK_d ) )
		this->velY = -64;
	else
		this->velY = 0;

	if( InputHandler::getKey( SDLK_g ) && this->grenade == NULL && grenadeCoolDown <= 0 && !this->dead )
	{
		if( score < 100 )
		{
			notEnoughScores = true;
			startTime = ticks;
		}
		else
		{
			this->grenade = new Grenade( 160, x, y, this->level );
			grenadeCoolDown = 30;
			score -= 100;
		}
	}
	else if( grenadeCoolDown > 0 )
		grenadeCoolDown -= 1 * dt;

	static bool down;

	if( startTime != 0 && ticks - startTime > 1.5 )
	{
		down = true;
	}

	if( down && ticks - startTime > 0.0 )
	{
		startTime += 2 * dt;
		if( ( ticks - startTime ) <= 0.0 )
			notEnoughScores = false;
	}

	if( InputHandler::getKey( SDLK_SPACE ) && shootCoolDown <= 0 && !this->dead )
	{
		bullets.push_back( new Bullet( this->x, this->y, 160, 0, level ) );
		shootCoolDown = 0.1;
	}
	else
		shootCoolDown -= 1 * dt;

	if( this->y > 82.4 - 32 )
		this->y = 50.4;
	else if( this->y < -82.4 + 32 )
		this->y = -50.4;

	for( std::vector<Bullet*>::iterator it = bullets.begin(); it != bullets.end(); )
	{
		(*it)->tick( dt );

		if( (*it)->getDead() )
		{
			explosions.push_back( new Explosion( (*it)->getPosX(), (*it)->getPosY(), 1 ) );

			delete (*it);

			it = bullets.erase( it );
		}
		else
			++it;
	}

	if( this->grenade != NULL )
	{
		if( this->grenade->getDead() )
		{
			delete grenade;

			grenade = NULL;
		}
		else
		{
			grenade->tick( dt );
		}
	}

	for( std::vector< Explosion* >::iterator it = explosions.begin(); it != explosions.end(); )
	{
		(*it)->time -= 1 * dt;

		(*it)->x -= 112 * dt;

		if( (*it)->time <= 0 )
		{
			delete (*it);

			it = explosions.erase( it );
		}
		else
			++it;
	}

	if( hurtCoolDown > 0 )
		hurtCoolDown -= 10 * dt;

	for( int i = 0; i < 3; i++ )
	{
		if( heartCoolDown[ i ] > 0 && heartCoolDown[ i ] <= 1 )
			heartCoolDown[ i ] -= 1 * dt;
	}
}

void Player::hurt( float damage )
{
	this->health -= damage;

	hurtCoolDown = 10;

	if( heartCoolDown[ 0 ] == 2 )
		heartCoolDown[ 0 ] = 1;
	else if( heartCoolDown[ 1 ] == 2 )
		heartCoolDown[ 1 ] = 1;
	else if( heartCoolDown[ 2 ] == 2 )
		heartCoolDown[ 2 ] = 1;
}

int Player::getMaxHealth()
{
	return 30;
}

void Player::render( Renderer& renderer )
{
	for( std::vector<Bullet*>::iterator it = bullets.begin(); it != bullets.end(); ++it )
	{
		(*it)->render( renderer );
	}

	if( this->grenade != NULL )
		grenade->render( renderer );

	for( std::vector< Explosion* >::iterator it = explosions.begin(); it != explosions.end(); ++it )
	{
		renderer.loadIdentity();

		renderer.translate( (*it)->x, (*it)->y, 0 );
		renderer.scale( 1.0, 1.0, 1.0 );
		renderer.rotate( 0, glm::vec3( 1, 1, 1 ) );

		renderer.drawSprite( 32, 32, 32, 0, glm::vec4( 255, 255, 255, 255 * ( ( (*it)->time < 1.0 ) ? (*it)->time : 1.0 ) ) );
	}

	renderer.loadIdentity();

	renderer.translate( this->x, this->y, 0 );
	renderer.scale( 1.0, 1.0, 1.0 );
	renderer.rotate( 0, glm::vec3( 1, 1, 1 ) );

	if( ( (int) hurtCoolDown % 2 ) != 0 )
		renderer.drawSprite( width, height, 16, 32, glm::vec4( 255, 63, 63, 255 ) );
	else
		renderer.drawSprite( width, height, 16, 32, glm::vec4( 255, 255, 255, 255 ) );

	renderer.loadIdentity();

	for( int i = 0; i < 3; i++ )
	{
		renderer.translate( -64 - i * 20, -54, 0 );
	
		glDisable( GL_DEPTH_TEST );

		renderer.drawSprite( 16, 16, 80, 0, glm::vec4( 255 * heartCoolDown[ i ], 255 * heartCoolDown[ i ], 255 * heartCoolDown[ i ], 255 ) );

		glEnable( GL_DEPTH_TEST );
	}

	glDisable( GL_DEPTH_TEST );
	
	if( notEnoughScores )
		renderer.drawText( "Not enough Points", glm::vec2( -51, 0 ), glm::vec4( 255, 255, 255, 255 * ( ( ticks - startTime < 1 ) ? ticks - startTime : 1 ) ) );
	
	renderer.loadIdentity();

	renderer.translate( 0.0, -54, 0.0 );
	renderer.scale( ( 30.0 / ( ( grenadeCoolDown < 1 ) ? 1 : grenadeCoolDown ) ) * 1.0, 4.0, 0.0 );

	renderer.drawQuad( glm::vec4( 31, 63, 31, 255 ) );

	std::string s = "Goal: ";

    std::string buf = std::to_string( goal );
	s += buf;

	s += "   Score: ";

    buf = std::to_string( score );
	s += buf;

	renderer.loadIdentity();

	renderer.drawText( s, glm::vec2( -96, 56 ), glm::vec4( 255, 255, 255, 255 ) );

	glEnable( GL_DEPTH_TEST );

}
