#ifndef INPUT_H
#define INPUT_H


#include "SDL2/SDL2/include/SDL.h"


class Input
{
public:
	Input();


	bool KeyIsPressed(int key);
	bool PadIsPressed(SDL_GameControllerButton key);


	// define the keys:
	static const int ARROW_LEFT = SDL_SCANCODE_LEFT;
	static const int ARROW_RIGHT = SDL_SCANCODE_RIGHT;
	static const int ARROW_UP = SDL_SCANCODE_UP;
	static const int ARROW_DOWN = SDL_SCANCODE_DOWN;

	static const int SPACE = SDL_SCANCODE_SPACE;



protected:
	;
private:

	SDL_GameController * controller;
	const Uint8 *m_keystate;
};
#endif