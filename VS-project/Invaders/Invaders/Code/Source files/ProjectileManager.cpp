#include "ProjectileManager.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

ProjectileManager::ProjectileManager()
{
	// Create a new vector for holding projectiles.
	m_projectiles = std::list<IProjectile*>(); 
}
ProjectileManager::~ProjectileManager()
{
	// We loop through the projectile list and ensure that we correctly
	// delete each item in it.
	while (!m_projectiles.empty()) {
		delete m_projectiles.back();
		m_projectiles.pop_back();
	}

	m_projectiles.clear();
}

void ProjectileManager::Update(float frameTime)
{
	std::list<IProjectile*>::iterator it = m_projectiles.begin();
	for ( it = m_projectiles.begin(); it != m_projectiles.end(); )
	{
		// Update this individual projectile.
		IProjectile* proj = static_cast<IProjectile*>(*it);
		proj->Update(frameTime);
		
		if ( !proj->IsAlive() )
		{
			// If that is the case, we must remove and delete it from the list.
			delete *it;
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
	std::list<IProjectile*>::iterator it = m_projectiles.begin();
	for ( it; it != m_projectiles.end(); ++it )
	{
		// Render the projectile.
		IProjectile* proj = static_cast<IProjectile*>(*it);
		proj->Render();
	}
}

void ProjectileManager::SpawnProjectile(IProjectile* proj)
{
	// Initialise and add the passed projectile to the list of projectiles.
	proj->Init();
	m_projectiles.push_back(proj);
}