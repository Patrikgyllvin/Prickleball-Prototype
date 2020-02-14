#ifndef SHADER_H
#define SHADER_H

#include <cstdio>
#include <cstdlib>

#include <string>
#include <vector>

#include <fstream>

#include <GL/GLEW.h>

#include <SDL2/SDL_opengl.h>

class Shader
{
public:
	Shader();
	Shader( const char* vertPath, const char* fragPath );
	
	~Shader();

	void loadAndCompile( const char* vertPath, const char* fragPath );

	GLuint getProgram();

private:
	GLuint programID;
};

#endif
