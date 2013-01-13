#include "Enemy.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

Enemy::Enemy(float xPos, float yPos, int row, int score) :
	m_alive(1),
	m_row(row),
	m_score(score)
{
	// Initialise the player's position.
	m_position = Position(xPos, yPos);
}
Enemy::~Enemy()
{
	// Destroy the sprite (which frees its memory).
	//mp_sprite->destroy();
	mp_sprite = 0;
}

void Enemy::Init()
{
	// Create the sprite for the enemy based on its row.
	const char* p_sprite = ( m_row < 2 ) ? "data/enemy1.bmp" : "data/enemy2.bmp";

	mp_sprite = Game::GetInstance().GetSystem().createSprite(p_sprite);
}

void Enemy::Update(float frameTime)
{

}

void Enemy::Move(int direction, float elapsedTime)
{
	
}

/*
void Enemy::Fire()
{
	
}
*/