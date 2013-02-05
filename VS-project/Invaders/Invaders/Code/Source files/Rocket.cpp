#include "Rocket.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

Rocket::Rocket(Vector2 position) :
	m_alive(false)
{
	m_spriteWidth = 8;
	m_spriteHeight = 16;
	m_spriteClipWidth = 6;
	m_spriteClipHeight = 16;

	// Create the sprite for the rocket.
	mp_sprite = Game::GetInstance().GetResourceManager().GetRocketSprite();

	// Initialise the position of the rocket based on the
	// Player's position.
	m_position = position;
}
Rocket::~Rocket()
{
}

void Rocket::Update(float frameTime)
{
	if ( m_alive )
	{
		float offset = frameTime * ROCKET_VELOCITY;
		m_position.y -= offset;

		// This represents the projectile going off-screen on the top.
		if ( m_position.y < ( 0 - m_spriteHeight ) )
		{
			Game::GetInstance().GetPlayer().KillRocket();
		}
	}
}

void Rocket::Render()
{
	// Use the Renderer to draw the rocket sprite.
	Game::GetInstance().GetRenderer().DrawSprite(
		mp_sprite, m_position.x, m_position.y, m_spriteWidth, m_spriteHeight
	);

#ifdef _DEBUG
	Vector2 boundPos = m_position;
	boundPos.x -= (32 - (m_spriteWidth/2));
	boundPos.y -= (32 - (m_spriteHeight/2));

	Game::GetInstance().GetRenderer().DEBUG_DrawBox(
		boundPos.x, boundPos.y, 64, 64, 
		Renderer::GetColour(0, 0, 255)
		);
#endif
}