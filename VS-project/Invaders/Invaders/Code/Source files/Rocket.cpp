#include "Rocket.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

Rocket::Rocket() :
	m_alive(true),
	m_spriteWidth(8),
	m_spriteHeight(16)
{
	m_spriteClipWidth = 6;
	m_spriteClipHeight = 19;
	m_spriteClipXOffset = 13;
	m_spriteClipYOffset = 7;

	// Create the sprite for the rocket.
	mp_sprite = Game::GetInstance().GetResourceManager().GetRocketSprite();

	// Initialise the position of the rocket based on the
	// Player's position.
	m_position = Game::GetInstance().GetPlayer().GetPosition();
	m_position.y += ROCKET_OFFSET;
}
Rocket::~Rocket()
{
	mp_sprite = NULL;

	// The player now has no rocket.
	Game::GetInstance().GetPlayer().KillRocket();
}

void Rocket::Update(float frameTime)
{
	if ( m_alive )
	{
		float offset = frameTime * ROCKET_VELOCITY;
		m_position.y -= offset;

		// This represents the projectile going off-screen on the top.
		
		// TODO!!!
		
		if ( m_position.y < ( 0 ) ) //- Game::GetInstance().GetSpriteSize() ) )
		{
			m_alive = false;
		}
	}
}

void Rocket::Render()
{
	// Use the Renderer to draw the rocket sprite.
	Game::GetInstance().GetRenderer().DrawSprite(
		mp_sprite, m_position.x, m_position.y, m_spriteWidth, m_spriteHeight
	);
}