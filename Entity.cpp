#include "Entity.h"

Entity::Entity( Level& level )
	:
x( 0 ),
y( 0 ),
z( 0 ),
velX( 0 ),
velY( 0 ),
velZ( 0 ),
rotX( 0 ),
rotY( 0 ),
rotZ( 0 ),
maxHealth( 0 ),
dead( false ),
ticks( 0 ),
width( 0 ),
height( 0 ),
level( level )
{
	this->health = maxHealth;
}

Entity::Entity( float x, float y, float z, float velX, float velY, float velZ, float rotX, float rotY, float rotZ, Level& level )
	:
x( x ),
y( y ),
z( z ),
velX( velX ),
velY( velY ),
velZ( velZ ),
rotX( rotX ),
rotY( rotY ),
rotZ( rotZ ),
dead( false ),
maxHealth( 0 ),
ticks( 0 ),
level( level )
{
	this->health = maxHealth;
}

Entity::~Entity()
{}

int Entity::getHealth()
{
	return this->health;
}

int Entity::getMaxHealth()
{
	return 20;
}

void Entity::hurt( float damage )
{
	this->health -= damage;
}

bool Entity::getDead()
{
	return this->dead;
}

int Entity::getTicks()
{
	return this->ticks;
}

void Entity::tick( float dt )
{
	ticks += 1 * dt;

	this->x += velX * dt;
	this->y += velY * dt;
	this->z += velZ * dt;

	if( this->getHealth() <= 0 )
		this->dead = true;

	this->onUpdate( dt );
}

void Entity::render( Renderer& renderer )
{}

void Entity::setPosX( double x )
{
	this->x = x;
}

void Entity::setPosY( double y )
{
	this->y = y;
}

void Entity::setPosZ( double z )
{
	this->z = z;
}

double Entity::getPosX()
{
	return this->x;
}

double Entity::getPosY()
{
	return this->y;
}

double Entity::getPosZ()
{
	return this->z;
}

void Entity::setRotX( double rotX )
{
	this->rotX = rotX;
}

void Entity::setRotY( double rotY )
{
	this->rotY = rotY;
}

void Entity::setRotZ( double rotZ )
{
	this->rotZ = rotZ;
}

double Entity::getRotX()
{
	return this->rotX;
}

double Entity::getRotY()
{
	return this->rotY;
}

double Entity::getRotZ()
{
	return this->rotZ;
}

void Entity::setVelX( double velX )
{
	this->velX = velX;
}

void Entity::setVelY( double velY )
{
	this->velY = velY;
}

void Entity::setVelZ( double velZ )
{
	this->velZ = velZ;
}

double Entity::getVelX()
{
	return this->velX;
}

double Entity::getVelY()
{
	return this->velY;
}

double Entity::getVelZ()
{
	return this->velZ;
}

double Entity::getWidth()
{
	return this->width;
}

double Entity::getHeight()
{
	return this->height;
}