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
		IRenderable();
		virtual ~IRenderable();

		// This pure virtual function marks this class as an abstract base
		// class.
		virtual void IsAbstract() = 0;

		// Draws the sprite.
		virtual void Render() { }

		//---------------------------------------------------------------------
		// Accessors
		inline Position GetPosition() { return m_position; }
		inline void SetPosition(Position newPos) { m_position = newPos; }

		// Note that we only need an accessor for the width, as all sprites
		// in the game are square.
		inline int GetSpriteBounds() { return SPRITE_WIDTH; }

		inline int GetXOffset() { return m_spriteClipXOffset; }
		inline int GetYOffset() { return m_spriteClipYOffset; }
		inline int GetWidth() { return m_spriteClipWidth; }
		inline int GetHeight() { return m_spriteClipHeight; }

	protected:
		// The position that the renderable object is currently at.
		Position m_position;

		// These variables are used for collision testing. It would look
		// very peculiar if projectiles collided with things they hadn't hit.
		int m_spriteClipWidth;
		int m_spriteClipHeight;
		int m_spriteClipXOffset;
		int m_spriteClipYOffset;

		// Pointer to the sprite object as defined in the DiceInvaders lib.
		ISprite* mp_sprite;
		static const int SPRITE_WIDTH = 32;
};

#endif // RENDERABLE_H