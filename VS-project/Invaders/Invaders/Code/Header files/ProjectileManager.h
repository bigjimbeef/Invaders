#ifndef PROJECTILEMANAGER_H
#define PROJECTILEMANAGER_H

//-----------------------------------------------------------------------------
// ProjectileManager.h
// Creates, updates, renders and destroys all projectiles within the game, both
// player- and enemy-based.
//-----------------------------------------------------------------------------

// Uses STL list for managing projectiles.
#include <list>

#include "Projectile.h"

// Forward declare Game to allow access to Game singleton.
class Game;

class ProjectileManager
{
	public:
		static ProjectileManager& GetInstance()
		{
			static ProjectileManager instance;
			return instance;
		}
		virtual ~ProjectileManager();

		void Update(float frameTime);

		void Render();

		// Spawn a projectile. The position and other properties of
		// the projectile will be detailed in the Projectile itself.
		void SpawnProjectile(Projectile* proj);

	private:

		// A list of all projectiles in the game world.
		std::list<Projectile*> m_projectiles;

		// Private default ctor to facilitate Singleton pattern.
		ProjectileManager();

		// DO NOT IMPLEMENT
		ProjectileManager(ProjectileManager const&);
		void operator=(ProjectileManager const&);
};

#endif // PROJECTILEMANAGER_H