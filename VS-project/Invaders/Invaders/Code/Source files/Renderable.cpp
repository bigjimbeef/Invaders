#include "Renderable.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

RenderableBase::RenderableBase() :
	m_position(0.0f, 0.0f),
	m_spriteWidth(0),
	m_spriteHeight(0),
	m_spriteClipWidth(0),
	m_spriteClipHeight(0),
	mp_sprite(NULL)
{

#ifdef _DEBUG
	m_DEBUG_Colliding = false;
#endif
}

RenderableBase::~RenderableBase()
{
}