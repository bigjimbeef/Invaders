#include "Renderable.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

IRenderable::~IRenderable()
{
	// Destroy the sprite and free its memory.
	mp_sprite->destroy();
	delete mp_sprite;
}

void IRenderable::Render()
{
	// Draw the sprite for this renderable item.
	mp_sprite->draw(int(m_position.x), int(m_position.y));
}