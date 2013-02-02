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
	frameTime /= 1000.0f;

	std::list<IProjectile*>::iterator it = m_projectiles.begin();
	for ( it = m_projectiles.begin(); it != m_projectiles.end(); )
	{
		// Update this individual projectile.
		(*it)->Update(frameTime);
		
		if ( !(*it)->IsAlive() )
		{
			if ( (*it)->IsEnemyProjectile() )
			{
				EnemyProjectile* p_proj = static_cast<EnemyProjectile*>(*it);
				// If this is a enemy projectile, we want to remove it from 
				// the list of projectiles that its owner has.
				p_proj->GetOwner().KillProjectile(*p_proj);
			}

			// If that is the case, we must remove and delete it from the list.
			delete *it;
			*it = NULL;
			it = m_projectiles.erase(it);
		}
		else
		{
			++it;
		}
	}

	/*
#ifdef _DEBUG
	int numProjectiles = m_projectiles.size();
	
	std::stringstream ss;
	ss << numProjectiles << " projectiles";
	std::string pos = ss.str();

	Game::GetInstance().GetSystem().drawText(0, 20, pos.c_str());
#endif
	*/
}

void ProjectileManager::Render()
{
	std::list<IProjectile*>::const_iterator it = m_projectiles.begin();
	for ( it; it != m_projectiles.end(); ++it )
	{
		// Render the projectile.
		(*it)->Render();
	}
}

void ProjectileManager::SpawnProjectile(IProjectile& proj)
{
	// Initialise and add the passed projectile to the list of projectiles.
	m_projectiles.push_back(&proj);
}

void ProjectileManager::SendCharacter(char letter)
{
	std::list<IProjectile*>::iterator it = m_projectiles.begin();
	for ( it; it != m_projectiles.end(); ++it )
	{
		if ( (*it)->IsAlive() )
		{
			if ( (*it)->IsEnemyProjectile() )
			{
				EnemyProjectile* p_proj = static_cast<EnemyProjectile*>(*it);
				
				Word* p_word = p_proj->GetWord();
				if ( p_word )
				{
					// Now send the letter to the actual word object.
					// Return val indicates if it killed the letter.
					bool removed = p_word->ReceiveLetter(letter);
					// We only want to kill one letter at once.
					if ( removed )
					{
						break;
					}
				}
			}
		}
	}
}