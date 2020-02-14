#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>

#include <SDL2/SDL_image.h>

#include <GL/glew.h>

#include <cstdio>

class Texture
{
public:
	Texture();
	Texture( const char* tex );

	~Texture();

	void loadTexture( const char* tFile );

	bool getLoaded();

	void bind();

	int getWidth();
	int getHeight();

private:
	GLuint texID;

	bool loaded;

	int w, h;
};

#endif