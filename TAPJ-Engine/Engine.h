#ifndef ENGINE_H
#define ENGINE_H


// SDL libraries:
#include "SDL2/SDL2/include/SDL.h"


// standard libraries:
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <time.h>

// engine libraries:
#include "Object.h"
#include "Input.h"
#include "Sprite.h"


using namespace std;


class Engine
{
public:
	static const int RENDERER_SOFTWARE = 0;
	static const int RENDERER_HARDWARE = 1;
	static const int RENDERER_OPENGL = 2;

	Engine();
	~Engine();

	// opens up a window:
	void OpenWindow(string title, int width, int height, int renderer);

	// sets the title of the window:
	void SetWindowTitle(string title);

	// destroys it:
	void DestroyWindow();

	// this function starts the engine's internal loop:
	void Run();


	// functions/classes used by the engine itself AND/OR the objects inside it:
	// timing functions:
	void Delay(int ms);		// pause the engine for MS amount of milliseconds
	float GetFrameTime();	// get the time between the current and the last frame

							// engine functions:
	void ClearWindow();		// clear the window area
	bool IsRunning();		// get the engine status
				
	int GetNextRandom(int min, int max);// random numbers:

	int GetNextID(); //return available ID

	// sprites:
	void LoadSprite(		// load a bitmap to the sprite POOL.
		string name,			// name of the sprite in the POOL
		string path,			// path of the file on disk
		bool transparent,		// image is transparent?
		bool sheet = false,		// image is sprite sheet?
		int horizontalCount = 0,// if so, how many sprites are inside it horizontally,
		int verticalCount = 0); // and how many vertically?

								// collision:
	vector<Object*> GetCollisions(string object); // returns a list of all (solid) objects, intersected with the object with the name 'object'

												  // objects:
	void AttachObject(string name, Object *o);  // add an object with name 'name' to the object POOL (queued and executed later)
	void ReleaseObject(string name);			// remove an object from the object POOL (queued and executed later)
	Object *FindObject(string name);			// return the object from the POOL with name 'name'

												// input:
	Input *input;				// objects can access this class to check the keyboard status

protected:
	;

private:
	//Object IDs
	int ids = 0;

	// object queues:
	queue<Object*> m_queueAttach;
	queue<Object*> m_queueRelease;

	// active object POOL:
	map<string, Object*> m_mapObjects;

	// sprite POOL:
	map<string, Sprite*> m_mapSprites;

	// a pointer to the sdl window:
	SDL_Window *m_window;
	SDL_Renderer* m_render;

	// a pointer to the screen surface:
	SDL_Surface *m_screen;

	// used in getFrameTime:
	Uint64 m_frameTimeNow;
	Uint64 m_frameTimeLast;
	float m_frameTime;

	// a flag indicating if the engine is running:
	bool m_isRunning;

	// these functions take the queued object actions and perform them:
	void AttachQueuedObjects();
	void ReleaseQueuedObjects();

	// updates the internal engine timer:
	void Update();

	// updates the state of the input:
	void HandleInput();

	// renderer flag:
	int m_renderFlag;

	//window variables
	int winWidth;
	int winHeight;
	float winScaleX;
	float winScaleY;

	/*// opengl-specific functions:
	SDL_GLContext m_context;

	void PrintProgramLog(GLuint program);
	void PrintShaderLog(GLuint shader);

	GLuint m_program;
	GLuint m_vertexShader;
	GLuint m_fragmentShader;

	GLuint m_vbo;
	GLuint m_vao;

	void SetupOpenGL();
	void SetupProgram();
	void SetupVertexShader();
	void SetupFragmentShader();*/
};

#endif

