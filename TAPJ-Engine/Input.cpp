#include "Input.h"



Input::Input()
{
	m_keystate = SDL_GetKeyboardState(NULL);
	for (int i = 0; i < SDL_NumJoysticks(); i++) {
		if (SDL_IsGameController(i)) {
			controller = SDL_GameControllerOpen(i);
			break;
		}
	}

}



bool Input::KeyIsPressed(int key)
{
	return (bool)m_keystate[key];
}

bool Input::PadIsPressed(SDL_GameControllerButton key)
{
	return (bool)SDL_GameControllerGetButton(controller, key);
}
