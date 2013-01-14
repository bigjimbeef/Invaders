#include "Bomb.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

Bomb::Bomb(Enemy& bombOwner) :
	m_bombOwner(bombOwner),
	m_alive(true)
{
    // TODO: Update
	m_spriteClipWidth = 6;
	m_spriteClipHeight = 19;
	m_spriteClipXOffset = 13;
	m_spriteClipYOffset = 7;
}
Bomb::~Bomb()
{
	mp_sprite = NULL;

	// TODO: Remove a single bomb from the enemy that fired it.
	// Game::GetInstance().GetPlayer().KillRocket();
}

void Bomb::Init()
{
	// Get the bomb sprite from the ResourceManager.
	mp_sprite = ResourceManager::GetBombSprite();

	// Initialise the position of the bomb based on the firing enemy's
	// position.
	m_position = m_bombOwner.GetPosition();
	m_position.y += BOMB_OFFSET;
}

void Bomb::Update(float frameTime)
{
	if ( m_alive )
	{
		float offset = frameTime * Bomb_VELOCITY;
		m_position.y -= offset;

		// This represents the projectile going off-screen at the bottom.
		if ( m_position.y > Game::GetInstance().GetScreenHeight() )
		{
			m_alive = false;
		}
	}
}

void Bomb::Render()
{
	mp_sprite->draw(int(m_position.x), int(m_position.y));
}
