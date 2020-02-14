#include "Level.h"

#include "Entity.h"

#include "Player.h"

#include "PrickleBall.h"

#include <iostream>

Level::Level()
:
width( 3 ),
height( 1 ),
numTiles( 0 ),
prickleBallCooldown( 0 ),
time( 90 ),
win( 0 )
{}

Level::~Level()
{
	for( int x = 0; x < width; x++ )
		for( int y = 0; y < height; y++ )
			delete map[ y * width + x ];
}

void Level::load( Renderer* renderer )
{
	this->renderer = renderer;
}

void Level::tick( float dt )
{
	if( time <= 0 && player->score >= player->goal )
		win = 1;
	else if( time <= 0 && player->score < player->goal )
		win = -1;
	else
	{
		win = 0;
		
		time -= 1 * dt;
	}

	if( player->getDead() )
	{
		win = -2;
	}

	if( win == 1 || win == -1 || win == -2 )
	{
		for( std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); )
		{
			delete (*it);

			it = entities.erase( it );
		}
	}

	for( int i = 0; i < map.size(); i ++ )
	{
		map[ i ]->setX( map[ i ]->getX() - 112 * dt );

		if( map[ i ]->getX() <= 0 && numTiles < 3 )
		{
			map.push_back( new BaseTile( 256, 160, 0, 0, true, 256 , 0 ) );

			numTiles++;
		}

		if( map[ i ]->getX() <= -256 )
		{
			delete map[ i ];
			map.erase( map.begin() + i );

			numTiles--;
		}
	}

	for( std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); )
	{
		(*it)->tick( dt );
		(*it)->setVelX( -112 );
		
		if( (*it)->getDead() || (*it)->getPosX() < -128 )
		{
			if( (*it)->getDead() && (*it)->getHealth() <= 0 )
				player->score += 200;
			else if( (*it)->getPosX() < -128 )
				player->goal += 50;
			
			delete (*it);

			it = entities.erase( it );
		}
		else
			++it;
	}

	int toSpawn = rand() % 11;
	int lastY = -60;

	if( prickleBallCooldown <= 0 )
	{
		prickleBallCooldown = rand() % 41;

		for( int i = 0; i < toSpawn; i++ )
		{
			entities.push_back( new PrickleBall( *this ) );
			
			int y = ( rand() % 176 ) - 80;
			while( y == lastY )
				y = ( rand() % 176 ) - 80;

			lastY = y;

			entities.back()->setPosX( 128 );
			entities.back()->setPosY( y );
		}
	}
	else
		prickleBallCooldown -= 10 * dt;
}

void Level::render()
{
	renderer->setTexture( "ground" );

	for( int i = 0; i < map.size(); i++ )
		map[ i ]->render( *this->renderer );

	renderer->setTexture( "sheet" );

	for( std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it )
	{
		(*it)->render( *this->renderer );
	}

	if( win == 1 )
	{
		renderer->loadIdentity();

		renderer->drawText( "YOU WON! fuckin' maniac -_-", glm::vec2( -81, 0 ), glm::vec4( 63, 255, 63, 255 ) );
	}
	else if( win == -1 )
	{
		renderer->loadIdentity();

		renderer->drawText( "YOU LOST! looooseeeerr! :O", glm::vec2( -78, 0 ), glm::vec4( 255, 63, 63, 255 ) );
	}
	else if( win == -2 )
	{
		renderer->loadIdentity();

		renderer->drawText( "YOU DIED!!!", glm::vec2( -33, 0 ), glm::vec4( 255, 63, 63, 255 ) );
	}
	else
	{
		renderer->loadIdentity();

        std::string buf = std::to_string( time );

		std::string s = "Time: "; s += buf;

		if( time <= 15 )
		{
			if( ( (int) time % 2 ) != 0 )
				renderer->drawText( s, glm::vec2( 54, -54 ), glm::vec4( 255, 63, 63, 255 ) );
			else
				renderer->drawText( s, glm::vec2( 54, -54 ), glm::vec4( 255, 255, 255, 255 ) );
		}
		else
			renderer->drawText( s, glm::vec2( 54, -54 ), glm::vec4( 255, 255, 255, 255 ) );
	}
}

Tile& Level::getTile( int x, int y )
{
    return *map[ y * width + x ];
}

int Level::getHeight( int x )
{
	for( int y = 0; y < height; y++ )
	{
		if( getTile( x, y ).getSolid() == false )
			return y - 1;
	}
	
	return -1;
}

std::vector<Tile*> Level::getColumn( int x )
{
	return std::vector<Tile*>(NULL);
}

void Level::loadLevel( Player* player )
{
    this->player = player;
	this->player->setPosX( 0 );
	this->player->setPosY( 1 );
	this->player->setPosZ( 0 );

	map.reserve( width );
	
	map.push_back( new BaseTile( 256, 160, 0, 0, true, 0, 0 ) );
	numTiles++;
}

Player& Level::getPlayer()
{
    return *this->player;
}

std::vector<Entity*> Level::getEntitiesWithinAABB( glm::vec2 start, glm::vec2 end )
{
	std::vector<Entity*> entitiesInAABB;

	for( std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it )
	{
		if( (*it)->getPosX() + (*it)->getWidth() / 2 > start.x &&
			(*it)->getPosX() - (*it)->getWidth() / 2 < end.x )
			if( (*it)->getPosY() - (*it)->getHeight() / 2 < start.y &&
				(*it)->getPosY() + (*it)->getHeight() / 2 > end.y )
				entitiesInAABB.push_back( (*it) );
	}

	return entitiesInAABB;
}
