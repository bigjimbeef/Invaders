#include "Enemy.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

Enemy::Enemy(float xPos, float yPos, int row, int col, int score) :
	m_alive(1),
	m_row(row),
	m_col(col),
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

void Enemy::Update(float frameTime, int direction, bool dropDown)
{
	// Make our move.
	Move(direction, frameTime, dropDown);
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

	mp_sprite->draw(int(m_position.x), int(m_position.y));
}

void Enemy::Move(int direction, float elapsedTime, bool dropDown)
{
	// If we're not dropping down, we move across the screen.
	if ( !dropDown )
	{
		m_position.x += EnemyManager::GetInstance().GetMoveDelta();

		// When we move, we attempt to fire a bomb.
	}
	else
	{
		m_position.y += EnemyManager::GetInstance().GetDropDistance();
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
}

void Enemy::Fire()
{
    if ( m_bombs.size() < MAX_BOMBS )
    {
        // We can fire up to three bombs at once, and have a chance to fire
        // one on move.
        int doFire = ( rand() % 100 ) + 1;

        if ( doFire < FIRING_CHANCE )
        {
            // Create a new bomb, initialise it, and add it to the list.
            Bomb* p_bomb = new Bomb(*this);
            p_bomb->Init();
            m_bombs.push_back(newBomb);
        }
    }
}
