#include "Renderable.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

IRenderable::IRenderable() :
	mp_sprite(NULL)
{
}

IRenderable::~IRenderable()
{
}