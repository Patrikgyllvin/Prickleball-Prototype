#ifndef PRICKLEBALL_H
#define PRICKLEBALL_H

#include "Entity.h"

#include "Player.h"

class PrickleBall : public Entity
{
public:
	PrickleBall( Level& level );
	~PrickleBall();

	void onUpdate( float dt );
	void render( Renderer& renderer );

	int getMaxHealth();
};

#endif