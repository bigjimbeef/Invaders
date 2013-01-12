#include "Player.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

Player::Player(float xPos, float yPos) :
	m_health(STARTING_HEALTH)
{
	// Initialise the player's position.
	m_position = Position(xPos, yPos);
}
Player::~Player()
{
	// Destroy the player sprite and free its memory.
	mp_sprite->destroy();
	delete mp_sprite;
}

void Player::Init()
{
	// Create the sprite for the player.
	mp_sprite = Game::GetInstance().GetSystem().createSprite("data/player.bmp");
}

void Player::Update(float frameTime)
{

}

void Player::Move(int direction, float elapsedTime)
{
	float offset = elapsedTime * PLAYER_VELOCITY;

	if ( direction == -1 )
	{
		// Moving to the left.
		offset = -offset;
	}

	m_position.x += offset;
}