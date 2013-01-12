#include "ProjectileManager.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

ProjectileManager::ProjectileManager()
{
	// Create a new vector for holding projectiles.
	m_projectiles = std::list<Projectile*>(); 
}
ProjectileManager::~ProjectileManager()
{
	// TODO: We need to loop through the projectile list and ensure
	// that we correctly delete each item in it.

	m_projectiles.clear();
}

void ProjectileManager::Update(float frameTime)
{
	std::list<Projectile*>::iterator it = m_projectiles.begin();
	for ( it = m_projectiles.begin(); it != m_projectiles.end(); )
	{
		// Update this individual projectile.
		Projectile* proj = static_cast<Projectile*>(*it);
		proj->Update(frameTime);
		
		if ( !proj->IsAlive() )
		{
			// If that is the case, we must remove and delete it from the list.
			delete *it;
			*it = 0;
			it = m_projectiles.erase(it);
		}
		else
		{
			++it;
		}
	}

#ifdef _DEBUG
	int numProjectiles = m_projectiles.size();
	
	std::stringstream ss;
	ss << numProjectiles << " projectiles";
	std::string pos = ss.str();

	Game::GetInstance().GetSystem().drawText(0, 20, pos.c_str());
#endif
}

void ProjectileManager::Render()
{
	std::list<Projectile*>::iterator it = m_projectiles.begin();
	for ( it; it != m_projectiles.end(); ++it )
	{
		// Render the projectile.
		Projectile* proj = static_cast<Projectile*>(*it);
		proj->Render();
	}
}

void ProjectileManager::SpawnProjectile(Projectile* proj)
{
	// Initialise and add the passed projectile to the list of projectiles.
	proj->Init();
	m_projectiles.push_back(proj);
}