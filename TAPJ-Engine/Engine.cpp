#include "Engine.h"

Engine::Engine()
{
	m_window = NULL;
	m_screen = NULL;

	m_isRunning = true;

	// initialize SDL2
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		cout << "Cannot initialize SDL2" << endl;
		return;
	}

	// create an input object:
	input = new Input();

	// set up the internal timer:
	m_frameTimeNow = SDL_GetPerformanceCounter();
	m_frameTimeLast = 0;
	m_frameTime = 0.0f;

	// set up the RNG:
	srand((unsigned int)time(NULL));
}



Engine::~Engine()
{
	DestroyWindow();
	SDL_Quit();
}



void Engine::OpenWindow(string title, int width, int height, int renderer)
{
	m_renderFlag = renderer;
	m_window = SDL_CreateWindow
	(
		title.c_str(),					   // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		width,                             // width, in pixels
		height,                            // height, in pixels
		SDL_WINDOW_OPENGL |                // flags - see below
		SDL_WINDOW_RESIZABLE
	);


	m_render = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	winWidth = width;
	winHeight = height;
	winScaleX = 1;
	winScaleY = 1;

	// Check that the window was successfully created
	if (m_window == NULL)
	{
		// In the case that the window could not be made...
		cout << "Could not create window: " << SDL_GetError() << endl;
		return;
	}
	
	m_screen = SDL_GetWindowSurface(m_window);
}



void Engine::SetWindowTitle(string title)
{
	SDL_SetWindowTitle(m_window, title.c_str());
}



void Engine::DestroyWindow()
{

	if (m_window != NULL)
	{
		SDL_DestroyWindow(m_window);
	}
}



void Engine::Delay(int ms)
{
	SDL_Delay(ms);
}



void Engine::AttachObject(string name, Object * o)
{
	o->SetEngine(this);
	o->SetName(name);
	m_queueAttach.push(o);
}



void Engine::ReleaseObject(string name)
{
	if (m_mapObjects.find(name) != m_mapObjects.end())
	{
		m_queueRelease.push(m_mapObjects[name]);
	}
	else
	{
		// warning: object was not found.
	}
}



Object * Engine::FindObject(string name)
{
	Object *object = NULL;

	if (m_mapObjects.find(name) != m_mapObjects.end())
	{
		object = m_mapObjects[name];
	}
	return object;
}



int Engine::GetNextRandom(int min, int max)
{
	return rand() % max + min;
}

//Returns available ID, if game goes forever will overflow int
int Engine::GetNextID()
{
	ids++;
	return ids;
}



void Engine::AttachQueuedObjects()
{
	while (!m_queueAttach.empty())
	{
		m_mapObjects[m_queueAttach.front()->GetName()] = m_queueAttach.front();
		m_mapObjects[m_queueAttach.front()->GetName()]->Create();
		m_queueAttach.pop();
	}
}



void Engine::ReleaseQueuedObjects()
{
	//check if queue has objects to release
	while (!m_queueRelease.empty() )
	{
		//check for uninitialized objects that somehow get here
		if (m_queueRelease.front()->GetPosY()>-100000) {
			if (m_mapObjects.find(m_queueRelease.front()->GetName()) != m_mapObjects.end())
			{
				// get the object 'to be destroyed':
				Object* obj = m_mapObjects[m_queueRelease.front()->GetName()];

				// call its 'Destroy()' function:
				obj->Destroy();

				// erase it from the map:
				m_mapObjects.erase(obj->GetName());

				// free the memory:
				delete obj;
			}
			else
			{
				// warning: object was not found.
			}
		}
		//remove from queue
		m_queueRelease.pop();
	}
}



float Engine::GetFrameTime()
{
	return m_frameTime;
}

void Engine::Update()
{
	// update the frame time:
	m_frameTimeLast = m_frameTimeNow;
	m_frameTimeNow = SDL_GetPerformanceCounter();
	m_frameTime = (float)((m_frameTimeNow - m_frameTimeLast) * 1000 / SDL_GetPerformanceFrequency());
	float fps = 1 / m_frameTime * 1000;

	cout << "FPS: " << fps << endl;
}



void Engine::HandleInput()
{
	//get events from sdl
	SDL_PumpEvents();
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_WINDOWEVENT)
		{
			// window closed
			if (event.window.event == SDL_WINDOWEVENT_CLOSE)
			{
				m_isRunning = false;
			}
			//window resized
			if (event.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				cout << "resize" << endl;
				int new_width = event.window.data1;
				int new_height = event.window.data2;

				winScaleX = (float)new_width / winWidth;
				winScaleY = (float)new_height / winHeight;
				cout << winScaleX << " , " << new_width << endl;
				cout << winScaleY << " , " << new_height << endl;

				m_screen = SDL_GetWindowSurface(m_window);
			}
		}
	}
}



void Engine::ClearWindow()
{
	if (m_renderFlag == Engine::RENDERER_SOFTWARE)
	{
		SDL_FillRect(m_screen, NULL, 0x000000);
	}
	if (m_renderFlag == Engine::RENDERER_HARDWARE)
	{
		//sdl renderer
	}
	if (m_renderFlag == Engine::RENDERER_OPENGL)
	{
		//openGL
	}
}


//main engine loop
void Engine::Run()
{
	while (IsRunning())
	{
		// handle objects:
		AttachQueuedObjects();

		// handle input:
		HandleInput();

		// clear screen:
		ClearWindow();

		// draw objects:
		for (map<string, Object*>::iterator it = m_mapObjects.begin(); it != m_mapObjects.end(); it++)
		{
			string name = it->first;
			it->second->Frame();

			// check if object is still there:
			if (m_mapObjects.find(name) == m_mapObjects.end())
			{
				// start from the beginning:
				it = m_mapObjects.begin();
				continue;
			}

			// if the object does not have a frame, dont render it
			if (it->second->GetDefaultSprite() == "")
			{
				continue;
			}

			if (m_renderFlag == Engine::RENDERER_SOFTWARE)
			{
				// get the default (active) sprite:
				SDL_Surface *frame = m_mapSprites[it->second->GetDefaultSprite()]->GetFrame(it->second->GetDefaultIndex());
				
				// scale the frame to windows size
				SDL_Surface* scaledFrame = SDL_CreateRGBSurface(frame->flags, frame->w * winScaleX, frame->h * winScaleY, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
				SDL_BlitScaled(frame, NULL, scaledFrame, NULL);
				frame = scaledFrame;

				// move the frame:
				SDL_Rect rect;
				rect.w = frame->w;
				rect.h = frame->h;
				rect.x = (int)(it->second->GetPosX() - it->second->GetCenterX());
				rect.y = (int)(it->second->GetPosY() - it->second->GetCenterY());

				// draw the frame:
				SDL_BlitSurface(frame, NULL, m_screen, &rect);
				//free temporary surface
				SDL_FreeSurface(scaledFrame);
			}
			else if (m_renderFlag == Engine::RENDERER_OPENGL)
			{
				//openGL
				
			}
			else
			{
				cout << "Error: Bad renderer flag." << endl;
			}
		}

		// update the window:
		if (m_renderFlag == Engine::RENDERER_SOFTWARE)
		{
			SDL_UpdateWindowSurface(m_window);
		}
		else if (m_renderFlag == Engine::RENDERER_OPENGL)
		{
			//update openGL
		}


		// update the engine:
		Update();

		// handle objects:
		ReleaseQueuedObjects();
	}

	// free memory:
	// free all (still active) objects:
	for (map<string, Object*>::iterator it = m_mapObjects.begin(); it != m_mapObjects.end(); it++)
	{
		ReleaseObject(it->first);
	}
	ReleaseQueuedObjects();

	// free all sprites:
	for (map<string, Sprite*>::iterator it = m_mapSprites.begin(); it != m_mapSprites.end(); it++)
	{
		delete it->second;
	}

	// unload the engine:
	SDL_Quit();
}



bool Engine::IsRunning()
{
	return m_isRunning;
}



void Engine::LoadSprite(string name, string path, bool transparent, bool sheet, int horizontalCount, int verticalCount)
{

	// check if this resource is already loaded:
	if (m_mapSprites.find(name) != m_mapSprites.end())
	{
		return;
	}

	// load the image:
	SDL_Surface *image = SDL_LoadBMP(path.c_str());
	if (image == NULL)
	{
		cout << "Unable to load image [" << path << "] Error: " << SDL_GetError() << endl;
		return;
	}

	Sprite *sprite = new Sprite();
	sprite->SetIsSheet(sheet);

	if (transparent) //set transparency
	{
		sprite->SetColorKey();
	}


	// it's a sprite sheet:
	if (sheet)
	{
		sprite->SetFrameCount(horizontalCount * verticalCount);
		// split the image into different frames:
		int frameWidth = (image->w) / horizontalCount;
		int frameHeight = (image->h) / verticalCount;

		SDL_Rect destRect = { 0, 0, frameWidth, frameHeight };
		int f = 0;

		for (int j = 0; j < verticalCount; j++)
		{
			for (int i = 0; i < horizontalCount; i++)
			{
				SDL_Rect sourceRect = { i*frameWidth, j*frameHeight, frameWidth, frameHeight };

				SDL_Surface *frame = SDL_CreateRGBSurface(0, frameWidth, frameHeight, 32, 0, 0, 0, 0);

				int result = SDL_BlitSurface(image, &sourceRect, frame, &destRect);
				if (result)
				{
					cout << "Error copying frame from steet." << endl;
					return;
				}

				sprite->SetFrame(f, frame);
				f++;
			}
		}

	}

	// it's a single sprite:
	else
	{
		sprite->SetFrameCount(1);
		sprite->SetFrame(0, image);
	}

	// put it into the POOL:
	m_mapSprites[name] = sprite;
}



vector<Object*> Engine::GetCollisions(string object)
{
	vector<Object*> list;

	if (m_mapObjects.find(object) == m_mapObjects.end())
	{
		return list;
	}

	if (!m_mapObjects[object]->GetIsSolid())
	{
		return list;
	}

	for (map<string, Object*>::iterator it = m_mapObjects.begin(); it != m_mapObjects.end(); it++)
	{
		// if the object is itself, dont check
		if (it->first == object)
		{
			continue;
		}
		// only check for collision if the object is solid:
		if (it->second->GetIsSolid())
		{
			Object *obj = FindObject(object);

			float x = (obj->GetPosX() - it->second->GetPosX()) * (obj->GetPosX() - it->second->GetPosX());
			float y = (obj->GetPosY() - it->second->GetPosY()) * (obj->GetPosY() - it->second->GetPosY());
			float r = (obj->GetRadius() + it->second->GetRadius());

			float distance = sqrt(x + y - r);

			// there was a collision:
			if (distance < 35.0f)
			{
				list.push_back(it->second);
			}
		}
	}
	return list;
}