#include "Player.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

Player::Player(float xPos, float yPos) :
	m_health(STARTING_HEALTH),
	mp_rocket(0)
{
	// Initialise the player's position.
	m_position = Position(xPos, yPos);
}
Player::~Player()
{
	// Destroy the sprite (which frees its memory).
	// mp_sprite->destroy();
}

void Player::Init()
{
	// Create the sprite for the player.
	mp_sprite = Game::GetInstance().GetSystem().createSprite("data/player.bmp");
}

void Player::Update(float frameTime)
{
	// TODO: Update player's health.
}

void Player::Move(int direction, float elapsedTime)
{
	float offset = elapsedTime * PLAYER_VELOCITY;

	if ( direction == -1 )
	{
		// Moving to the left.
		offset = -offset;
	}

	// Update the player's position
	m_position.x += offset;
}

void Player::Fire()
{
	// We only fire if we don't already have a rocket in motion.
	if ( mp_rocket == 0 )
	{
		// Create a new rocket ...
		mp_rocket = new Rocket();
		// ... and spawn it with the ProjectileManager.
		ProjectileManager::GetInstance().SpawnProjectile(mp_rocket);
	}
}