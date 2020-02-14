#include "Camera.h"

Camera::Camera()
	:
pos( 0, 0, 0 ),
rot( 0, 0, 0 )
{}

Camera::Camera( glm::vec3 pos )
{
	Camera();

	setPos( pos );
}

Camera::Camera( float x, float y, float z )
{
	Camera( glm::vec3( x, y, z ) );
}

Camera::Camera( const Camera& cam )
{
	this->pos = cam.pos;
	this->rot = cam.rot;
}

Camera::~Camera()
{}

void Camera::setPos( glm::vec3 pos )
{
	this->pos = pos;
}

void Camera::setPos( float x, float y, float z )
{
	this->pos = glm::vec3( x, y, z );
}

void Camera::setRot( glm::vec3 rot )
{
	this->rot = rot;
}

void Camera::setRot( float rotX, float rotY, float rotZ )
{
	this->rot = glm::vec3( rotX, rotY, rotZ );
}

glm::vec3 Camera::getPos()
{
	return this->pos;
}

void Camera::getPos( float& x, float& y, float& z )
{
	x = this->pos.x;
	y = this->pos.y;
	z = this->pos.z;
}

float Camera::getPosX()
{
	return this->pos.x;
}

float Camera::getPosY()
{
	return this->pos.y;
}

float Camera::getPosZ()
{
	return this->pos.z;
}

glm::vec3 Camera::getRot()
{
	return this->rot;
}

void Camera::getRot( float& rotX, float& rotY, float& rotZ )
{
	rotX = this->rot.x;
	rotY = this->rot.y;
	rotZ = this->rot.z;
}

float Camera::getRotX()
{
	return this->rot.x;
}

float Camera::getRotY()
{
	return this->rot.y;
}

float Camera::getRotZ()
{
	return this->rot.z;
}