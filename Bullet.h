#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"

class Bullet : public Entity
{
public:
	Bullet( float x, float y, float velX, float velY, Level& level );
	Bullet( float x, float y, float z, float velX, float velY, float velZ, float rotX, float rotY, float rotZ, Level& level );
	virtual ~Bullet();

	virtual void onUpdate( float dt );
	virtual void render( Renderer& renderer );
};

#endif