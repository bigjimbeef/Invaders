#include "Renderable.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

IRenderable::IRenderable() :
	m_position(0.0f, 0.0f),
	m_spriteWidth(0),
	m_spriteHeight(0),
	m_spriteClipWidth(0),
	m_spriteClipHeight(0),
	m_spriteClipXOffset(0),
	m_spriteClipYOffset(0),
	mp_sprite(NULL)
{
}

IRenderable::~IRenderable()
{
}