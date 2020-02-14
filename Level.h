#ifndef LEVEL_H
#define LEVEL_H

#include "Renderer.h"

#include <vector>

#include <stdlib.h>

#include "Tile.h"
#include "BaseTile.h"

class Entity;

class Player;

typedef std::vector<Entity*> ENTITY_LIST;

class Level
{
public:
    Level();
    ~Level();

	template<class T>
    void createTile( int texW, int texH, int texPosX, int texPosY, bool solid, int x, int y )
    {
		delete map[ y * width + x ];

		map[ y * width + x ] = new T( texW, texH, texPosX, texPosY, solid, x, y );
    }

    Tile& getTile( int x, int y );

    void loadLevel( Player* player );

    void load( Renderer* renderer );
    void tick( float dt );
	void render();

    Player& getPlayer();

	std::vector<Entity*> getEntitiesWithinAABB( glm::vec2 start, glm::vec2 end );

private:
	int prickleBallCooldown;

	float time, win;

    std::vector<Tile*> map;

	int numTiles;

	int getHeight( int x );
	std::vector<Tile*> getColumn( int x );

    int width;
	int height;

    Renderer* renderer;
    
    Player* player;

    ENTITY_LIST entities;
};

#endif // LEVEL_H
