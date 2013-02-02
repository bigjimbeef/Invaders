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

		virtual void Update(float frameTime) { }
		virtual void Render() { }

		// This is an abstract base class. Mark it as such with pure virtual.
		virtual void IsAbstract() = 0;

		// Easily determines if the derived IProjectile is an EnemyProjectile
		// for casting.
		virtual bool IsEnemyProjectile() const { return false; }

		//---------------------------------------------------------------------
		// Accessors
		virtual inline bool IsAlive() const { return m_alive; }
		virtual inline void Kill() { m_alive = false; }
	
	protected:

		// Has the projectile hit anything or gone off-screen?
		bool m_alive;

};

#endif // PROJECTILE_H