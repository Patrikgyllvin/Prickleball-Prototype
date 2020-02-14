#include "Texture.h"

Texture::Texture()
{}

Texture::Texture( const char* tex )
{
	Texture();

	loadTexture( tex );
}

Texture::~Texture()
{
	glDeleteTextures( 1, &texID );
}

void Texture::loadTexture( const char* tFile )
{
	SDL_Surface* tex = IMG_Load( tFile );

	loaded = false;
	w = 0;
	h = 0;

	if( !tex )
	{
		fprintf( stderr, "Error: could not load texture %s, reason: %s\n", tFile, IMG_GetError() );
		return;
	}

	if( tex->w & ( tex->w - 1 ) || tex->h & ( tex->h - 1 ) )
	{
		fprintf( stderr, "Error: texture is not power of two!\n" );
		return;
	}

	GLenum texFmt;
	GLint bpp = tex->format->BytesPerPixel;

	if( bpp == 4 )
	{
		if( tex->format->Rmask == 0x000000ff )
			texFmt = GL_RGBA;
		else
			texFmt = GL_BGRA;
	}
	else if( bpp == 3 )
	{
		if( tex->format->Rmask == 0x0000ff )
			texFmt = GL_RGB;
		else
			texFmt = GL_BGR;
	}
	else
	{
		fprintf( stderr, "Cannot load texture, Not True-Color!\n" );
		return;
	}

	w = tex->w;
	h = tex->h;
	
	loaded = true;

	int GLVers;
	SDL_GL_GetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, &GLVers );

	glGenTextures( 1, &texID );
	bind();

	GLfloat maxA;
	if( GLVers >= 3 )
	{
		glGetFloatv( GL_TEXTURE_MAX_ANISOTROPY_EXT, &maxA );
	}

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, tex->w, tex->h, 0, texFmt, GL_UNSIGNED_BYTE, tex->pixels );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	if( GLVers >= 3 )
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
	else
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

	if( GLVers >= 3 )
	{
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxA );

		glGenerateMipmap( GL_TEXTURE_2D );
	}

	SDL_FreeSurface( tex );
}

void Texture::bind()
{
	glBindTexture( GL_TEXTURE_2D, texID );
}

bool Texture::getLoaded()
{
	return loaded;
}

int Texture::getWidth()
{
	return w;
}

int Texture::getHeight()
{
	return h;
}
