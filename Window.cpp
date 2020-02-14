#include "Window.h"
#include <iostream>

Window::Window()
	:
w( 640 ),
h( 360 ),
inputHandler( new InputHandler() ),
game( new Game() ),
renderer( new Renderer( game->getCamera() ) ),
OGL3( false )
{}

Window::~Window()
{
	delete inputHandler;
	delete game;
	delete renderer;
	
	SDL_GL_DeleteContext( glContext );
	SDL_DestroyWindow( window );
}

int Window::setupWindow()
{
	if( SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 ) != 0 ) fprintf( stderr, "Error: could not set red buffer: %s\n", SDL_GetError() );
	if( SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 ) != 0 ) fprintf( stderr, "Error: could not set green buffer: %s\n", SDL_GetError() );
	if( SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 ) != 0 ) fprintf( stderr, "Error: could not set blue buffer: %s\n", SDL_GetError() );
	if( SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 ) != 0 ) fprintf( stderr, "Error: could not set alpha buffer: %s\n", SDL_GetError() );
	if( SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 ) != 0 ) fprintf( stderr, "Error: could not set depth buffer: %s\n", SDL_GetError() );
	if( SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 ) != 0 ) fprintf( stderr, "Error: could not set stencil buffer: %s\n", SDL_GetError() );
	if( SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 ) != 0 ) fprintf( stderr, "Error: Multisampling not available: %s\n", SDL_GetError() );
	if( SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 4 ) != 0 ) fprintf( stderr, "Error: Multisampling not available: %s\n", SDL_GetError() );
	if( SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 4 ) != 0 ) fprintf( stderr, "Error: Double buffering not available: %s\n", SDL_GetError() );

	if( SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 ) != 0 ) fprintf( stderr, "Error: could not set OpenGL major version 3: %s\n", SDL_GetError() );
	if( SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 ) != 0 ) fprintf( stderr, "Error: could not set OpenGL minor version 3: %s\n", SDL_GetError() );
	if( SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE ) != 0 ) fprintf( stderr, "Error: OpenGL core profile not available: %s\n", SDL_GetError() );
	
#ifdef _DEBUG
	if( !( window = SDL_CreateWindow( "Prickly Ball Mania", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE ) ) )
#else
	if( !( window = SDL_CreateWindow( "Prickly Ball Mania", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_OPENGL ) ) )
#endif
	{
		fprintf( stderr, "Error: could not create SDL window: %s\n", SDL_GetError() );
		return -1;
	}

	SDL_GetWindowSize( window, &w, &h );

	if( !( glContext = SDL_GL_CreateContext( window ) ) )
	{
		fprintf( stderr, "Error: could not create OpenGL context: %s\n", SDL_GetError() );
		return -1;
	}

	std::string fullVers = std::string( (const char*)glGetString( GL_VERSION ) );
	int OGLMajVer = atoi( fullVers.substr( 0, 1 ).c_str() );
	int OGLMinVer = atoi( fullVers.substr( 2, 1 ).c_str() );

	if( OGLMajVer > 3 )
		OGL3 = true;
	else if( OGLMajVer == 3 && OGLMinVer >= 3 )
		OGL3 = true;
	else
		OGL3 = false;

	if( !OGL3 )
	{
		fprintf( stdout, "OpenGL 3.3 not supported on your system, trying OpenGL 1.1!\n" );
		
		SDL_GL_DeleteContext( glContext );

		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 1 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY );

		glContext = SDL_GL_CreateContext( window );
	}

	if( SDL_GL_SetSwapInterval( 1 ) != 0 ) fprintf( stderr, "Error: could not enable V-Sync: %s\n", SDL_GetError() );

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if( err != GLEW_OK )
	{
		fprintf( stderr, "Error: could not intialize GLEW: %s\n", glewGetErrorString( err ) );
		return -1;
	}

	return 0;
}

int Window::setupGL()
{
	glViewport( 0, 0, w, h );

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
	return 0;
}

void Window::run()
{
	load();

	long frames = 0;
	long lastSecond = SDL_GetTicks();

	Uint32 currTime = SDL_GetTicks();
	Uint32 oldTime;

	float delta;

	running = true;
	paused = false;
	
	SDL_Event e;

	while( running )
	{
		oldTime = currTime;
		currTime = SDL_GetTicks();
		
		delta = ( currTime - oldTime ) / 1000.0f;

		tick( delta );

		render();

		frames++;

		if( currTime - lastSecond > 1000 )
		{
			fprintf( stdout, "FPS: %d\n", frames );
			frames = 0;
			lastSecond = currTime;
		}

		do {
			while( SDL_PollEvent( &e ) )
				processEvents( e );
			if( paused ) currTime = SDL_GetTicks();
		} while( paused );
	}
}

void Window::processEvents( SDL_Event e )
{
	switch( e.type )
	{
	case SDL_QUIT:
		running = false;
		break;
	case SDL_WINDOWEVENT:
		switch( e.window.event )
		{
		case SDL_WINDOWEVENT_RESIZED:
			SDL_GetWindowSize( window, &w, &h );
			glViewport( 0, 0, w, h );
			renderer->changePerspective( w, h );
			break;
		}
		break;
	case SDL_KEYDOWN:
		if( e.key.keysym.sym == SDLK_ESCAPE )
		{
			this->running = false;
			this->paused = false;
		}
		if( e.key.keysym.sym == SDLK_p && !paused )
			this->paused = true;
		else if( e.key.keysym.sym == SDLK_p && paused )
			this->paused = false;
		break;
	}

	inputHandler->handleInput( e );
}

void Window::load()
{
	renderer->load( OGL3, w, h );
	game->load( renderer );
}

void Window::tick( float delta )
{
	game->tick( delta );
}

void Window::render()
{
	game->render();
	
	SDL_GL_SwapWindow( window );
}

int main( int argc, char** argv )
{
	Window window;

	srand( time( NULL ) );
	
	SDL_Init( SDL_INIT_EVERYTHING );

	IMG_Init( IMG_INIT_PNG );

	if( window.setupWindow() != 0 )
	{
		fprintf( stderr, "Error: could not setup window!\n" );
		return -1;
	}

	if( window.setupGL() != 0 )
	{
		fprintf( stderr, "Error: could not setup OpenGL!\n" );
		return -1;
	}

	window.run();

	return 0;
}