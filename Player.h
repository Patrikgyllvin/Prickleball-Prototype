#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

#include "InputHandler.h"

#include "Bullet.h"
#include "Grenade.h"

struct Explosion {
	Explosion( float x, float y, float time )
		:
	x( x ),
	y( y ),
	time( time )
	{}

	float x, y;
	float time;
};

class Player : public Entity
{
public:
	Player( Camera& cam, Level& level );
	~Player();

	void onUpdate( float dt );
	void render( Renderer& renderer );

	void hurt( float damage );

	int getMaxHealth();
	
	unsigned int score, goal;

private:
	Camera& cam;

	std::vector<Bullet*> bullets;

	Grenade* grenade;

	std::vector< Explosion* > explosions;

	float shootCoolDown, hurtCoolDown, heartCoolDown[ 3 ], grenadeCoolDown;

	bool notEnoughScores;
	float startTime;
};

#endif