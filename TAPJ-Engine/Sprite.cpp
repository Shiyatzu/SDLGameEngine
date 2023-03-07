#include "Sprite.h"



Sprite::Sprite()
{
	m_isTransparent = false;
}



Sprite::~Sprite()
{
	for (vector<SDL_Surface*>::iterator it = m_frames.begin(); it != m_frames.end(); it++)
	{
		SDL_FreeSurface((*it));
	}

	/*for (vector<GLuint>::iterator it = m_ids.begin(); it != m_ids.end(); it++)
	{
		glDeleteTextures(1, &(*it));
	}*/

}



bool Sprite::GetIsSheet()
{
	return m_isSheet;
}



void Sprite::SetIsSheet(bool b)
{
	m_isSheet = b;
}



int Sprite::GetFrameCount()
{
	return m_frameCount;
}



void Sprite::SetFrameCount(int c)
{
	m_frameCount = c;
	m_frames.clear();
	m_frames.resize(c);

	/*m_ids.clear();
	m_ids.resize(c);*/
}



void Sprite::SetFrame(int index, SDL_Surface * frame)
{
	// software:
	if (index >= (int)m_frames.size())
	{
		cout << "Frame index is outside bounds." << endl;
		return;
	}

	if (m_isTransparent)
	{
		SDL_SetColorKey(frame, SDL_TRUE, SDL_MapRGB(frame->format, 255, 0, 255));
	}

	m_frames[index] = frame;

	/*// opengl:
	if (index >= (int)m_ids.size())
	{
		cout << "Frame index is outside bounds." << endl;
		return;
	}*/

	/*GLuint id = 0;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame->w, frame->h, 0, GL_RGB, GL_UNSIGNED_BYTE, frame->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	m_ids[index] = id;*/

}


void Sprite::SetColorKey()
{
	m_isTransparent = true;
}



SDL_Surface * Sprite::GetFrame(int index)
{
	if (index >= (int)m_frames.size())
	{
		cout << "Frame index is outside bounds." << endl;
		return NULL;
	}

	return m_frames[index];
}



/*GLuint Sprite::GetTextureId(int index)
{
	if (index >= (int)m_ids.size())
	{
		cout << "Frame index is outside bounds." << endl;
		return NULL;
	}

	return m_ids[index];
}*/