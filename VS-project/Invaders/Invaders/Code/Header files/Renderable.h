#ifndef RENDERABLE_H
#define RENDERABLE_H

//-----------------------------------------------------------------------------
// Renderable.h
// Exposes an abstract base class IRenderable which is the basis for all 
// renderable elements of the game.
//-----------------------------------------------------------------------------

#include "Vector2.h"

struct IDirect3DTexture9;

class IRenderable
{
	public:
		IRenderable();
		virtual ~IRenderable();

		// This pure virtual function marks this class as an abstract base
		// class.
		virtual void IsAbstract() = 0;

		virtual void Update(float frameTime) { }
		virtual void Render() { }

		// Easily determines if the derived IProjectile is an EnemyProjectile
		// for casting.
		virtual bool IsEnemyProjectile() const { return false; }

		//---------------------------------------------------------------------
		// Accessors
		inline Vector2 GetPosition() const { return m_position; }
		inline void SetPosition(Vector2 newPos) { m_position = newPos; }

		inline int GetSpriteWidth() const { return m_spriteWidth; }
		inline int GetSpriteHeight() const { return m_spriteHeight; }

		inline int GetClipWidth() const { return m_spriteClipWidth; }
		inline int GetClipHeight() const { return m_spriteClipHeight; }

#ifdef _DEBUG
		inline void DEBUG_SetColliding(bool value){m_DEBUG_Colliding = value;}
#endif

	protected:
		// The position that the renderable object is currently at.
		Vector2 m_position;

		int m_spriteWidth;
		int m_spriteHeight;

		// These variables are used for collision testing. It would look
		// very peculiar if projectiles collided with things they hadn't hit.
		int m_spriteClipWidth;
		int m_spriteClipHeight;

		// Pointer to the sprite object as defined in the DiceInvaders lib.
		IDirect3DTexture9* mp_sprite;

#ifdef _DEBUG
		// DEBUG variables.
		bool m_DEBUG_Colliding;
#endif
};

#endif // RENDERABLE_H