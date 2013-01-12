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

void Player::Render()
{
	mp_sprite->draw(int(m_position.x), int(m_position.y));
}

void Player::Move(int direction, float elapsedTime)
{
	float offset = elapsedTime * PLAYER_VELOCITY;

	switch ( direction )
	{
		case 1: // Moving right.
			break;
		case -1: // Moving left.
			offset = -offset;
			break;
		default:
			break;
	}

	m_position.x += offset;
}