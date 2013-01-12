#include "Rocket.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

Rocket::Rocket() :
	m_alive(true)
{
}
Rocket::~Rocket()
{
	// Destroy the sprite (which frees its memory).
	mp_sprite->destroy();

	// The player now has no rocket.
	Game::GetInstance().GetPlayer().KillRocket();
}

void Rocket::Init()
{
	// Create the sprite for the player.
	mp_sprite = Game::GetInstance().GetSystem().createSprite("data/rocket.bmp");

	// Initialise the position of the rocket based on the
	// Player's position.
	m_position = Game::GetInstance().GetPlayer().GetPosition();
	m_position.y += ROCKET_OFFSET;
}

void Rocket::Update(float frameTime)
{
	float offset = frameTime * ROCKET_VELOCITY;
	m_position.y -= offset;

	// This represents the projectile going off-screen on the top.
	if ( m_position.y < ( 0 - SPRITE_WIDTH ) )
	{
		m_alive = false;
	}
}