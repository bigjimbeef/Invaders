#include "Enemy.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

ISprite* Enemy::s_enemySprite1 = NULL;
ISprite* Enemy::s_enemySprite2 = NULL;

Enemy::Enemy(float xPos, float yPos, int row, int col, int score) :
	m_alive(1),
	m_row(row),
	m_col(col),
	m_score(score)
{
	// Initialise the player's position.
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

void Enemy::Update(float frameTime)
{
	
}

void Enemy::Render()
{
	mp_sprite->draw(int(m_position.x), int(m_position.y));
}

void Enemy::Move(int direction, float elapsedTime)
{
	
}

void Enemy::Kill()
{
	// When the enemy is killed, we need to check if it is in the current
	// outside column. If it is, we need to check if there are any more
	// in the column. If not, we update the EnemyManager's column width
	// so that it can correctly find the window boundaries for movement.
	if ( m_col == 0 
	  || m_col == EnemyManager::GetInstance().GetNumRemainingCols() - 1 )
	{
		// Pass the column width calculations back to the EnemyManager.
		EnemyManager::GetInstance().CalculateNewColWidth();
	}

	// This enemy is no longer alive.
	m_alive = false;
}

/*
void Enemy::Fire()
{
	
}
*/