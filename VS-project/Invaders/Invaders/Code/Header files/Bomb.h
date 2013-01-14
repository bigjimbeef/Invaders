#ifndef BOMB_H
#define BOMB_H

//-----------------------------------------------------------------------------
// Bomb.h
// Is a projectile which enemies fire. It can only collide with the player,
// which will destroy the bomb, and decrement the player's lives.
//-----------------------------------------------------------------------------

// Include our base class.
#include "Projectile.h"

// Forward declare Game to allow access to Game singleton.
class Game;

class Bomb : public IProjectile
{
	public:
		// The rocket doesn't need parameters to its default ctor, as it
		// initialises its position based on the player.
		Bomb(Enemy& bombOwner);
		virtual ~Bomb();

		virtual void Init();

		virtual void Update(float frameTime);
		virtual void Render();

		// Implement this function to show that this class is not abstract.
		inline void IsAbstract() { /* Do nothing */ }

		//---------------------------------------------------------------------
		// Accessors

		inline bool IsAlive() { return m_alive; }
		inline void Kill() { m_alive = false; }

	private:
	    // The enemy that fired this bomb.
	    Enemy& m_bombOwner;

		static const int BOMB_VELOCITY = 350;
		static const int BOMB_OFFSET = 20;

	protected:
		bool m_alive;

};

#endif // BOMB_H