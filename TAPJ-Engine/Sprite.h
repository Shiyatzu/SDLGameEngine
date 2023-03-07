#ifndef SPRITE_H
#define SPRITE_H



#include "SDL2/SDL2/include/SDL.h"
#include <vector>
#include <iostream>

using namespace std;



class Sprite
{
public:
	Sprite();
	~Sprite();



	bool GetIsSheet();
	void SetIsSheet(bool b);

	int GetFrameCount();
	void SetFrameCount(int c);

	void SetFrame(int index, SDL_Surface *frame);

	void SetColorKey();

	// software rendering:
	SDL_Surface *GetFrame(int index);

	/*// opengl rendering:
	GLuint GetTextureId(int index);*/

protected:
	;
private:
	// if the sprite has many frames:
	bool m_isSheet;
	int  m_frameCount;

	// a list of all frames (or 1 frame is the sprite is not a sheet):
	vector<SDL_Surface*> m_frames;

	// if th sprite has transparency:
	bool m_isTransparent;

	/*// opengl:
	vector<GLuint> m_ids;*/
};

#endif