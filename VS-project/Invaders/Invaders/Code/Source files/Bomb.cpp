#include "Bomb.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

Bomb::Bomb(Enemy& bombOwner) :
	m_bombOwner(bombOwner),
	m_alive(true)
{
	m_spriteWidth =8;
	m_spriteHeight =16;
	m_spriteClipHeight = 8;
	m_spriteClipWidth = 16;

	// Get the bomb sprite from the ResourceManager.
	mp_sprite = Game::GetInstance().GetResourceManager().GetBombSprite();

	// Initialise the position of the bomb based on the firing enemy's
	// position.
	m_position = m_bombOwner.GetPosition();
	
	// TODO: OFFSET BOMBS BY ENEMY POSITION
}
Bomb::~Bomb()
{
	mp_sprite = NULL;

	// Deletion of the Bomb* in m_bombs is handled by ProjectileManager.
}

void Bomb::Update(float frameTime)
{
	if ( m_alive )
	{
		float offset = frameTime * BOMB_VELOCITY * Game::GetInstance().GetSpeedFactor();
		m_position.y += offset;

		// This represents the projectile going off-screen at the bottom.
		if ( m_position.y > Renderer::GetScreenHeight() )
		{
			m_alive = false;
		}
	}
}

void Bomb::Render()
{
	// Use the Renderer to draw the bomb sprite.
	Game::GetInstance().GetRenderer().DrawSprite(
		mp_sprite, m_position.x, m_position.y, m_spriteWidth, m_spriteHeight
	);
}
