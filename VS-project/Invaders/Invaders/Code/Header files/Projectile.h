#ifndef PROJECTILE_H
#define PROJECTILE_H

//-----------------------------------------------------------------------------
// Projectile.h
// The base class for both player and enemy projectiles, which maintains
// commonality between their functioning.
//-----------------------------------------------------------------------------

// Include our base class.
#include "Renderable.h"

class Projectile : IRenderable
{
	public:
		Projectile(float xPos = 0.0f, float yPos = 0.0f);
		virtual ~Projectile();

		void Update();

		// This is an abstract base class. Mark it as such with pure virtual.
		virtual void IsAbstract() = 0;

	private:

		// Has the projectile hit anything or gone off-screen?
		bool m_alive;

};

#endif // PROJECTILEMANAGER_H