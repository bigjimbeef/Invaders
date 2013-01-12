#include "Rocket.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

Rocket::Rocket() :
	m_alive(true)
{
}
Rocket::~Rocket()
{
}

void Rocket::Init()
{
	// Create the sprite for the player.
	mp_sprite = Game::GetInstance().GetSystem().createSprite("data/rocket.bmp");

	// Initialise the position of the rocket based on the
	// Player's position.
	m_position = Game::GetInstance().GetPlayer().GetPosition();
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