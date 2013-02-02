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
		ProjectileManager();
		~ProjectileManager();

		void Update(float frameTime);

		void Render();

		// Spawn a projectile. The position and other properties of
		// the projectile will be detailed in the Projectile itself.
		void SpawnProjectile(IProjectile& proj);

		// Intercepts a letter character from the keyboard, and sends it to
		// any EnemyProjectiles.
		void SendCharacter(char letter);

		//---------------------------------------------------------------------
		// Accessors
		inline const std::list<IProjectile*>& 
			GetProjectileList() const { return m_projectiles; }

	private:

		// A list of all projectiles in the game world.
		std::list<IProjectile*> m_projectiles;
};

#endif // PROJECTILEMANAGER_H