#ifndef RENDERABLE_H
#define RENDERABLE_H

//-----------------------------------------------------------------------------
// Renderable.h
// Exposes an abstract base class IRenderable which is the basis for all 
// renderable elements of the game.
//-----------------------------------------------------------------------------

#include "Position.h"

struct ISprite;

class IRenderable
{
	public:
		virtual ~IRenderable();

		// This pure virtual function marks this class as an abstract base
		// class.
		virtual void IsAbstract() = 0;

		//---------------------------------------------------------------------
		// Accessors
		inline Position GetPosition() { return m_position; }
		inline void SetPosition(Position newPos) { m_position = newPos; }

		// Note that we only need an accessor for the width, as all sprites
		// in the game are square.
		inline int GetSpriteWidth() { return SPRITE_WIDTH; }

		// Draws the sprite.
		virtual void Render();

	protected:
		// The position that the renderable object is currently at.
		Position m_position;

		// Pointer to the sprite object as defined in the DiceInvaders lib.
		ISprite* mp_sprite;
		static const int SPRITE_WIDTH = 32;
};

#endif // RENDERABLE_H