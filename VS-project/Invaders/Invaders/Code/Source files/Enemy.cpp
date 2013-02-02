#include "Enemy.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

Enemy::Enemy(float xPos, float yPos, int width, int height, 
			 int row, int col, int score) :
	m_alive(1),
	m_canFire(0),
	m_gettingAngry(0),
	m_anger(0),
	m_currentJitter(0.0f, 0.0f),
	m_jitterTimer(0.0f),
	m_baseJitterWait(0.5f),
	m_row(row),
	m_col(col),
	m_dropDistance(0.0f),
	m_score(score),
	m_altSprite(0)
{
	m_spriteWidth= width;
	m_spriteHeight = height;
	m_spriteClipWidth = ( row < 2 ) ? 32 : 44;
	m_spriteClipHeight = ( row < 2 ) ? 32 : 32;

    // Create a std::list for holding the bombs.
    m_bombs = std::list<Bomb*>();

	// Initialise the enemy's position.
	m_position = Vector2(xPos, yPos);

	mp_sprite = ( m_row < 2 ) ? 
		Game::GetInstance().GetResourceManager().GetEnemyOneSprite() :
		Game::GetInstance().GetResourceManager().GetEnemyTwoSprite();
}
Enemy::~Enemy()
{
	mp_sprite = NULL;
}

void Enemy::Update(float frameTime)
{
	// No need to process the rest of the function if we can't fire.
	if ( !m_canFire )
	{
		return;
	}

	// Ensure we can't fire too many bombs.
    if ( m_bombs.size() < MAX_BOMBS )
    {
		// We randomly pick a number in 5000.
		// If the number exceeds our anger threshold (that is, the number which
		// defines the percentage chance we'll start getting angry), we begin
		// getting ANGRY. And thus jittering.

		// TODO: Probably a better way of choosing whether or not to fire?
		int random = ( rand() % 5001 );
		if ( random <= ANGER_THRESHOLD )
		{
			// Shake it, baby.
			m_gettingAngry = true;
			m_anger = 1;
		}

		if ( m_gettingAngry )
		{
			// Handle the jittering of the enemy as it gets ready to fire.
			Jitter(frameTime);

			// If we're at full anger ...
			if ( m_anger >= MAX_ANGER )
			{
				// ... then fire!
				Fire();

				m_gettingAngry = false;
				m_anger = 0;
				m_currentJitter = Vector2();
			}
		}
    }
}

void Enemy::Render()
{
	float xPos = m_position.x + m_currentJitter.x;
	float yPos = m_position.y + m_currentJitter.y;

	// TODO TODO TODO TODO TODO TODO TODO TODO TODO 
	// This needs to be extended slightly to allow for drawing the sprites in their background colour.
	// Either that, or find out why there appear to be white artifacts around them when drawn in white.
	// TODO TODO TODO TODO TODO TODO TODO TODO TODO 

	DWORD targetCol = ( m_row < 2 ) ? 
		Renderer::GetColour(0,255,0) : 
		Renderer::GetColour(255,0,0);

	// Use the Renderer to draw the sprite in place.
	Game::GetInstance().GetRenderer().DrawSprite(
		mp_sprite, xPos, yPos, m_spriteWidth, m_spriteHeight
		
		// TODO
		//, 0.0f, targetCol
	);

#ifdef _DEBUG
	DWORD col = m_DEBUG_Colliding 
		? Renderer::GetColour(0, 255, 0)
		: Renderer::GetColour(255, 0, 0);

	Game::GetInstance().GetRenderer().DEBUG_DrawBox(
		xPos, yPos, m_spriteWidth, m_spriteHeight, col
	);
#endif
}

void Enemy::Move(float distance, bool dropDown)
{
	// If we're not dropping down, we move across the screen.
	if ( !dropDown )
	{
		// Scale our movement by the game speed factor.
		m_position.x += ( distance * Game::GetInstance().GetSpeedFactor() );
	}
	else
	{
		int drop = EnemyManager::GetDropDistance();
		m_dropDistance += drop;
		m_position.y += drop;
		
		// Just set this for every enemy. Faster than checking if it's
		// the same with an if statement.
		Game::GetInstance().GetEnemyManager().SetProgress(m_dropDistance);
	}
}

void Enemy::Jitter(float frameTime)
{
	float currentJitterDur = static_cast<float>(m_baseJitterWait / m_anger);

	if ( m_jitterTimer > currentJitterDur )
	{
		// Get a little angrier every time we fidget.
		m_anger++;

		int halfOffset = MAX_JITTER_OFFSET / 2;

		float randomX =
			static_cast<float>(( rand() % MAX_JITTER_OFFSET ) - halfOffset);
		float randomY =
			static_cast<float>(( rand() % MAX_JITTER_OFFSET ) - halfOffset);

		// Scale jitter speed by current anger of this enemy.
		m_currentJitter = Vector2(randomX, randomY);

		m_jitterTimer = 0.0f;
	}

	m_jitterTimer += ( frameTime * Game::GetInstance().GetSpeedFactor() );
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
	if ( m_col == Game::GetInstance().GetEnemyManager().GetMaxCol()
	  || m_col == Game::GetInstance().GetEnemyManager().GetMinCol() )
	{
		// Pass the column width calculations back to the EnemyManager.
 		Game::GetInstance().GetEnemyManager().CalculateNewColWidth();
	}

	// Get the maximum row for this column. If this enemy is the maximum,
	// we need to make the EnemyManager calculate the new maximum.
	if ( m_row == Game::GetInstance().GetEnemyManager().GetMaxRow(m_col) )
	{
		Game::GetInstance().GetEnemyManager().CalculateNewMaxRow(m_col);
	}
}

void Enemy::KillBomb(const Bomb& dyingBomb)
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
    Game::GetInstance().GetProjectileManager().SpawnProjectile(*p_bomb);
            
	// We also manage the list internally too, though the memory
	// management is handled in the ProjectileManager.
	m_bombs.push_back(p_bomb);
}