#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <SDL2/SDL.h>

#include <map>

class InputHandler
{
public:
	InputHandler();
	~InputHandler();

	void handleInput( SDL_Event e );
	
	static bool getKey( SDL_Keycode keyCode );
	static void setKey( SDL_Keycode key, bool state );
private:
	static std::map< int, bool > keys;
	
	void onKeyDown( SDL_Keycode keyCode );
	void onKeyUp( SDL_Keycode keyCode );
};

#endif