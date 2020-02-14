#ifndef RENDERER_H
#define RENDERER_H

#include "MathHelper.h"

#include "Shader.h"

#include "Texture.h"

#include "Camera.h"
#include <GL/GLEW.h>
#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef struct {
	GLfloat x, y, z;
} Vertex;

typedef struct {
	GLubyte r, g, b, a;
} Color;

typedef struct {
	GLfloat u, v;
} UVs;

class Renderer
{
	friend class Window;

public:
	Renderer( Camera& camera );

	~Renderer();

	void load( bool OGL3, int w, int h );

	void translate( glm::vec3 pos );
	void translate( float x, float y, float z );

	void rotate( float angle, glm::vec3 axis );

	void scale( glm::vec3 scale );
	void scale( float x, float y, float z );

	void loadIdentity();

	void drawSpriteBillboarded( glm::vec3 pos, glm::vec3 lookAtPos, int w, int h, int uP, int vP, glm::vec4 col );
	void drawSprite( int w, int h, int uP, int vP, glm::vec4 col );

	void drawQuad( glm::vec4 col );

	void drawCube( int w, int h, int uP, int vP, glm::vec4 col );

	void drawText( std::string text, glm::vec2 pos, glm::vec4 col );

	void setTexture( std::string tex );

private:
	void changePerspective( int w, int h );

	glm::mat4 transMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 scaleMatrix;

	glm::mat4 camTransMatrix;
	glm::mat4 camRotationXMatrix;
	glm::mat4 camRotationYMatrix;
	glm::mat4 camRotationZMatrix;

	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 modelViewProjectionMatrix;

	bool OGL3;

	GLuint winW, winH;

	GLuint vao, quad, cube, UV, color;

	bool isShaderSTD, isShaderNoTex;

	Shader shader, shaderNoTex;
	Texture texture, ground;

	GLuint MVPLocation, texMatLoc, MVPLocationNoTex;

	Camera& cam;
};

#endif
