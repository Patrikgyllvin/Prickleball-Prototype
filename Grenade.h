#ifndef Grenade_H
#define Grenade_H

#include "Entity.h"

class Grenade : public Entity
{
public:
	Grenade( float velX, float x, float y, Level& level );
	~Grenade();

	void onUpdate( float dt );
	void render( Renderer& renderer );

	bool getExploded();

	float getExplosionCoolDown();

private:
	bool exploded;

	float explosionCoolDown;
};

#endif