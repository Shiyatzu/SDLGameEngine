#include "Object.h"

Object::Object()
{
	m_name = "obj";

	m_posX = 0.0f;
	m_posY = 0.0f;
	m_centerX = 0.0f;
	m_centerY = 0.0f;

	m_isSolid = false;
}



Object::~Object()
{
	;
}



void Object::Create()
{
	// this method shoud be overriden.
}



void Object::Frame()
{
	// this method shoud be overriden.
}



void Object::Destroy()
{
	// this method shoud be overriden.
}



void Object::SetEngine(Engine * e)
{
	m_engine = e;
}



Engine * Object::GetEngine()
{
	return m_engine;
}



void Object::SetPos(float x, float y)
{
	m_posX = x;
	m_posY = y;
}



float Object::GetPosX()
{
	return m_posX;
}



float Object::GetPosY()
{
	return m_posY;
}



void Object::SetCenter(float x, float y)
{
	m_centerX = x;
	m_centerY = y;
}



float Object::GetCenterX()
{
	return m_centerX;
}



float Object::GetCenterY()
{
	return m_centerY;
}



void Object::SetIsSolid(bool s)
{
	m_isSolid = s;
}



bool Object::GetIsSolid()
{
	return m_isSolid;
}



void Object::SetRadius(float r)
{
	m_radius = r;
}



float Object::GetRadius()
{
	return m_radius;
}



string Object::GetDefaultSprite()
{
	return m_defaultSprite;
}



int Object::GetDefaultIndex()
{
	return m_defaultIndex;
}



void Object::SetName(string n)
{
	m_name = n;
}



string Object::GetName()
{
	return m_name;
}



void Object::SetDefaultSprite(string s, int index)
{
	m_defaultSprite = s;
	m_defaultIndex = index;
}

