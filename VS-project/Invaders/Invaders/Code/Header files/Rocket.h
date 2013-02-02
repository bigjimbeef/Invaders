#ifndef ROCKET_H
#define ROCKET_H

//-----------------------------------------------------------------------------
// Rocket.h
// Is a projectile which the player fires. It can only collide with enemies,
// which will destroy both objects.
//-----------------------------------------------------------------------------

// Include our base class.
#include "Projectile.h"

// Forward declare Game to allow access to Game singleton.
class Game;

class Rocket : public IProjectile
{
	public:
		// The rocket doesn't need parameters to its default ctor, as it
		// initialises its position based on the player.
		Rocket();
		virtual ~Rocket();

		virtual void Update(float frameTime);
		virtual void Render();

		// Implement this function to show that this class is not abstract.
		inline void IsAbstract() { /* Do nothing */ }

		//---------------------------------------------------------------------
		// Accessors
		inline bool IsAlive() const { return m_alive; }
		inline void Kill() { m_alive = false; }

	private:
		static const int ROCKET_VELOCITY = 350;
	
		static const int ROCKET_OFFSET = -20;

	protected:
		bool m_alive;
};

#endif // ROCKET_H