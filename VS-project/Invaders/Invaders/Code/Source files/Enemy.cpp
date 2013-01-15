#include "Enemy.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

Enemy::Enemy(float xPos, float yPos, int row, int col, int score) :
	m_alive(1),
	m_canFire(0),
	m_bombTimer(0.0f),
	m_bombFireTarget(0.0f),
	m_row(row),
	m_col(col),
	m_dropDistance(0.0f),
	m_score(score),
	m_altSprite(0)
{
    // Create a std::list for holding the bombs.
    m_bombs = std::list<Bomb*>();

	// Initialise the enemy's position.
	m_position = Position(xPos, yPos);

	m_spriteClipWidth = ( row < 2 ) ? 24 : 30;
	m_spriteClipHeight = ( row < 2 ) ? 24 : 22;
	m_spriteClipXOffset = ( row < 2 ) ? 4 : 1;
	m_spriteClipYOffset = ( row < 2 ) ? 4 : 5;
}
Enemy::~Enemy()
{
	mp_sprite = NULL;
}

void Enemy::Init()
{
	mp_sprite = ( m_row < 2 ) ? 
		ResourceManager::GetEnemyOneSprite() :
		ResourceManager::GetEnemyTwoSprite();
}

void Enemy::Update(const float frameTime)
{
	// No need to process the rest of the function if we can't fire.
	if ( !m_canFire )
	{
		return;
	}

	// Ensure we can't fire too many bombs.
    if ( m_bombs.size() < MAX_BOMBS )
    {
		if ( m_bombFireTarget == 0.0f )
		{
			m_bombFireTarget = static_cast<float>(MIN_FIRE_TIME);
			
			// Random no. from 0-100
			int random = ( rand() % 101 );
			float randomPercentOffset =
				static_cast<float>(MAX_FIRE_TIME) * ( random / 100.0f );

			// Ensure we can't go below the minimum fire time.
			m_bombFireTarget = max(m_bombFireTarget, randomPercentOffset);
		}

		// Increment the timer.
		m_bombTimer += frameTime;

		// If we've reached the timer duration.
        if ( m_bombTimer > m_bombFireTarget )
        {
			// Attempt to fire a bomb after a random timer has elapsed.
			Fire();

			// Reset the timer variables.
			m_bombFireTarget = 0.0f;
			m_bombTimer = 0.0f;
        }
    }
}

void Enemy::Render()
{
	if ( m_row < 2 )
	{
		mp_sprite = m_altSprite 
			? ResourceManager::GetEnemyOneSprite() 
			: ResourceManager::GetEnemyOneAltSprite();
	}
	else 
	{
		mp_sprite = m_altSprite 
			? ResourceManager::GetEnemyTwoSprite() 
			: ResourceManager::GetEnemyTwoAltSprite();
	}

	mp_sprite->draw(static_cast<int>(m_position.x),
					static_cast<int>(m_position.y));
}

void Enemy::Move(const bool dropDown)
{
	// If we're not dropping down, we move across the screen.
	if ( !dropDown )
	{
		m_position.x += EnemyManager::GetInstance().GetMoveDelta();
	}
	else
	{
		int drop = EnemyManager::GetDropDistance();
		m_dropDistance += drop;
		m_position.y += drop;
		
		// Just set this for every enemy. Faster than checking if it's
		// the same with an if statement.
		EnemyManager::GetInstance().SetProgress(m_dropDistance);
	}

	m_altSprite = !m_altSprite;
}

void Enemy::Kill()
{
	// This enemy is no longer alive.
	m_alive = false;

	// Add to the player's score.
	GameState::GetInstance().IncrementScore(m_score);

	// When the enemy is killed, we need to check if it is in the current
	// outside column. If it is, we need to check if there are any more
	// in the column. If not, we update the EnemyManager's column width
	// so that it can correctly find the window boundaries for movement.
	if ( m_col == EnemyManager::GetInstance().GetMaxCol()
	  || m_col == EnemyManager::GetInstance().GetMinCol() )
	{
		// Pass the column width calculations back to the EnemyManager.
 		EnemyManager::GetInstance().CalculateNewColWidth();
	}

	// Get the maximum row for this column. If this enemy is the maximum,
	// we need to make the EnemyManager calculate the new maximum.
	if ( m_row == EnemyManager::GetInstance().GetMaxRow(m_col) )
	{
		EnemyManager::GetInstance().CalculateNewMaxRow(m_col);
	}
}

void Enemy::KillBomb(Bomb& dyingBomb)
{
	std::list<Bomb*>::iterator bombIt = m_bombs.begin();
	for ( bombIt; bombIt != m_bombs.end(); ++bombIt )
	{
		Bomb* p_bomb = static_cast<Bomb*>(*bombIt);
		// Compare the mem locations of the bombs.
		if ( p_bomb == &dyingBomb )
		{
			// Remove the bomb from the local array.
			// (deletion is handled in projectile manager)
			bombIt = m_bombs.erase(bombIt);
			
			// Don't need to continue.
			return;
		}
	}
}

void Enemy::Fire()
{
	// Create a new bomb, then spawn it with the projectile manager.
    Bomb* p_bomb = new Bomb(*this);
    ProjectileManager::GetInstance().SpawnProjectile(*p_bomb);
            
	// We also manage the list internally too, though the memory
	// management is handled in the ProjectileManager.
	m_bombs.push_back(p_bomb);
}