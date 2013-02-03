#include "EnemyProjectile.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

EnemyProjectile::EnemyProjectile(Enemy& projOwner) :
	m_projOwner(projOwner),
	m_alive(true),
	mp_word(NULL)
{
	m_spriteWidth = 8;
	m_spriteHeight = 16;
	m_spriteClipHeight = 8;
	m_spriteClipWidth = 16;

	// Get the projectile sprite from the ResourceManager.
	mp_sprite = 
		Game::GetInstance().GetResourceManager().GetEnemyProjectileSprite();

	// Initialise the position of the projectile based on the firing enemy's
	// position.
	m_position = m_projOwner.GetPosition();
	m_position.x +=
		(m_projOwner.GetSpriteWidth() - m_spriteWidth) / 2;
	m_position.y +=
		(m_projOwner.GetSpriteHeight() 
		+ m_projOwner.GetClipHeight() - m_spriteHeight) / 2;

	// If we're in the game's second (main) mode.
	if ( GameState::GetInstance().InMainGameMode() ) 
	{
		mp_word = new Word(*this);
	}
}
EnemyProjectile::~EnemyProjectile()
{
	mp_sprite = NULL;

	// Deletion of the EnemyProjectile* in m_projectiles is handled by 
	// the ProjectileManager.

	// Delete the Word, if we have one.
	if ( mp_word != NULL )
	{
		delete mp_word;
		mp_word = NULL;
	}
}

void EnemyProjectile::Update(float frameTime)
{
	if ( m_alive )
	{
		float offset = frameTime * PROJ_VELOCITY * Game::GetInstance().GetSpeedFactor();
		m_position.y += offset;

		// If we have a Word, then update it.
		if ( mp_word != NULL )
		{
			mp_word->Update(frameTime);
		}

		// This represents the projectile going off-screen at the bottom.
		if ( m_position.y > Renderer::GetScreenHeight() )
		{
			m_alive = false;
		}
	}
}

void EnemyProjectile::Render()
{
	// We want to draw the standard projectile sprite for the first part of the
	// game, then draw the letter projectile when we have one.

	if ( mp_word == NULL )
	{
		Game::GetInstance().GetRenderer().DrawSprite(
			mp_sprite, m_position.x, m_position.y, m_spriteWidth, m_spriteHeight
		);
	}
	else
	{
		// Render the word.
		mp_word->Render();
	}

#ifdef _DEBUG
	Vector2 boundPos = m_position;
	boundPos.x -= (32 - (m_spriteWidth/2));
	boundPos.y -= (32 - (m_spriteHeight/2));

	Game::GetInstance().GetRenderer().DEBUG_DrawBox(
		boundPos.x, boundPos.y, 64, 64,
		Renderer::GetColour(255, 0, 255)
	);
#endif
}
