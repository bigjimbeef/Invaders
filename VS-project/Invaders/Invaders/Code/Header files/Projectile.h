#ifndef PROJECTILE_H
#define PROJECTILE_H

//-----------------------------------------------------------------------------
// Projectile.h
// The abstract base class for both player and enemy projectiles, which
// maintains commonality between their functioning.
//-----------------------------------------------------------------------------

// Include our base class.
#include "Renderable.h"

class IProjectile : public IRenderable
{
	public:
		IProjectile();
		virtual ~IProjectile();

		virtual void Init() { }

		virtual void Update(float frameTime) { }
		virtual void Render() { }

		// This is an abstract base class. Mark it as such with pure virtual.
		virtual void IsAbstract() = 0;

		//---------------------------------------------------------------------
		// Accessors
		virtual inline bool IsAlive() { return m_alive; }
		virtual inline void Kill() { m_alive = false; }
	
	protected:

		// Has the projectile hit anything or gone off-screen?
		bool m_alive;

};

#endif // PROJECTILEMANAGER_H