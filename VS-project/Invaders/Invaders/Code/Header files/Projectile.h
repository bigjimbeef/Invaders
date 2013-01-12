#ifndef PROJECTILE_H
#define PROJECTILE_H

//-----------------------------------------------------------------------------
// Projectile.h
// The abstract base class for both player and enemy projectiles, which
// maintains commonality between their functioning.
//-----------------------------------------------------------------------------

// Include our base class.
#include "Renderable.h"

class Projectile : public IRenderable
{
	public:
		virtual ~Projectile();

		virtual void Init() { }

		virtual void Update(float frameTime) { }

		// This is an abstract base class. Mark it as such with pure virtual.
		virtual void IsAbstract() = 0;

		//---------------------------------------------------------------------
		// Accessors
		virtual inline bool IsAlive() { return m_alive; }
	
	protected:

		// Has the projectile hit anything or gone off-screen?
		bool m_alive;

};

#endif // PROJECTILEMANAGER_H