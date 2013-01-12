#ifndef PROJECTILEMANAGER_H
#define PROJECTILEMANAGER_H

//-----------------------------------------------------------------------------
// ProjectileManager.h
// Creates, updates, renders and destroys all projectiles within the game, both
// player- and enemy-based.
//-----------------------------------------------------------------------------

// Uses STL vector for managing projectiles.
#include <vector>

class ProjectileManager
{
	public:
		static ProjectileManager& GetInstance()
		{
			static ProjectileManager instance;
			return instance;
		}
		virtual ~ProjectileManager();

		void Update();

		void Render();


	private:

		std::vector<Projectile*> m_projectiles;


		// Private default ctor to facilitate Singleton pattern.
		ProjectileManager();

		// DO NOT IMPLEMENT
		ProjectileManager(ProjectileManager const&);
		void operator=(ProjectileManager const&);
};

#endif // PROJECTILEMANAGER_H