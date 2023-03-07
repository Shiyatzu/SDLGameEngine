#ifndef OBJECT_H
#define OBJECT_H


#include <iostream>


using namespace std;

class Engine;




class Object
{
public:
	Object();
	~Object();

	// called once when object is created:
	virtual void Create();

	// called every frame:
	virtual void Frame();

	// called once when object is destroyed:
	virtual void Destroy();

	// a way to connect with the engine:
	void SetEngine(Engine *e);
	Engine* GetEngine();

	// functions controlling the position of the object:
	void  SetPos(float x, float y);
	float GetPosX();
	float GetPosY();
	void  SetCenter(float x, float y);
	float GetCenterX();
	float GetCenterY();

	// functions used in collision detection:
	void SetIsSolid(bool s);
	bool GetIsSolid();
	void SetRadius(float r);
	float GetRadius();

	// functions controlling the default sprite of the object:
	void   SetDefaultSprite(string s, int index = 0);
	string GetDefaultSprite();
	int    GetDefaultIndex();

	// set and get the object's name:
	void SetName(string n);
	string GetName();

protected:
	;

private:
	// the name of the object itself (same as the object POOL):
	string m_name;

	// the object position:
	float m_posX;
	float m_posY;

	// the objects' center point:
	float m_centerX;
	float m_centerY;

	// used in collisions:
	bool  m_isSolid;
	float m_radius;

	// a pointer to the engine. set up by the engine.
	// if the object want's to use some engine method,
	// it can just call GetEngine()-> :
	Engine *m_engine;

	// the object's default sprite and index:
	string m_defaultSprite;
	int    m_defaultIndex;
};


#endif