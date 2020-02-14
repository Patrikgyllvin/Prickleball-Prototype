#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
	Camera();

	Camera( glm::vec3 pos );
	Camera( float x, float y, float z );

	Camera( const Camera& cam );

	~Camera();

	void setPos( glm::vec3 pos );
	void setPos( float x, float y, float z );

	void setRot( glm::vec3 rot );
	void setRot( float rotX, float rotY, float rotZ );

	glm::vec3 getPos();
	void getPos( float& x, float& y, float& z );

	float getPosX();
	float getPosY();
	float getPosZ();

	glm::vec3 getRot();
	void getRot( float& rotX, float& rotY, float& rotZ );

	float getRotX();
	float getRotY();
	float getRotZ();
	
private:
	glm::vec3 pos, rot;
};

#endif