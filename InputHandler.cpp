#include "InputHandler.h"

std::map< int, bool > InputHandler::keys;

InputHandler::InputHandler()
{}

InputHandler::~InputHandler()
{}

void InputHandler::handleInput( SDL_Event e )
{
	switch( e.type )
	{
	case SDL_KEYDOWN:
		if( e.key.repeat == 0 )
			onKeyDown( e.key.keysym.sym );
		break;
	case SDL_KEYUP:
		if( e.key.repeat == 0 )
			onKeyUp( e.key.keysym.sym );
		break;
	}
}

void InputHandler::onKeyDown( SDL_Keycode keyCode )
{
	keys[ keyCode ] = true;
}

void InputHandler::onKeyUp( SDL_Keycode keyCode )
{
	keys[ keyCode ] = false;
}

bool InputHandler::getKey( SDL_Keycode keyCode )
{
	return keys[ keyCode ] == true;
}

void InputHandler::setKey( SDL_Keycode key, bool state )
{
	keys[ key ] = state;
}