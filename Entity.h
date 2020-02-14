#ifndef ENTITY_H
#define ENTITY_H

#include "Renderer.h"

#include "Level.h"

class Entity
{
public:
	Entity( Level& level );
	Entity( float x, float y, float z, float velX, float velY, float velZ, float rotX, float rotY, float rotZ, Level& level );

	virtual ~Entity();

	void tick( float dt );
	virtual void render( Renderer& renderer );

	int getHealth();

	bool getDead();

	int getTicks();

	virtual void hurt( float damage );

	void setPosX( double x );
	void setPosY( double y );
	void setPosZ( double z );

	void setVelX( double velX );
	void setVelY( double velY );
	void setVelZ( double velZ );

	double getPosX();
	double getPosY();
	double getPosZ();

	double getVelX();
	double getVelY();
	double getVelZ();

	void setRotX( double rotX );
	void setRotY( double rotY );
	void setRotZ( double rotZ );
	
	double getRotX();
	double getRotY();
	double getRotZ();

	double getWidth();
	double getHeight();

protected:
	int maxHealth;
	int health;

	float ticks;

	bool dead;

	double x, y, z, width, height;
	double velX, velY, velZ;

	double rotX, rotY, rotZ;

	virtual void onUpdate( float dt ) = 0;
	virtual int getMaxHealth();

	Level& level;
};

#endif
