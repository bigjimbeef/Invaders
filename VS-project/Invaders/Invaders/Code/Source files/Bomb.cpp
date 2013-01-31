#include "Bomb.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

Bomb::Bomb(Enemy& bombOwner) :
	m_bombOwner(bombOwner),
	m_alive(true)
{
	m_spriteClipWidth = 8;
	m_spriteClipHeight = 16;
	m_spriteClipXOffset = 12;
	m_spriteClipYOffset = 8;

	// Get the bomb sprite from the ResourceManager.
	mp_sprite = ResourceManager::GetBombSprite();

	// Initialise the position of the bomb based on the firing enemy's
	// position.
	m_position = m_bombOwner.GetPosition();
	m_position.y += BOMB_OFFSET;
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
		if ( m_position.y > Game::GetScreenHeight() )
		{
			m_alive = false;
		}
	}
}

void Bomb::Render()
{
	mp_sprite->draw(static_cast<int>(m_position.x),
					static_cast<int>(m_position.y));
}
