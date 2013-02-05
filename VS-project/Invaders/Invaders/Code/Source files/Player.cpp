#include "Player.h"

// Needed for abs
#include <cmath>
#include <list>

// Include within .cpp file to match forward declare in .h
#include "Game.h"

Player::Player() :
	m_health(STARTING_HEALTH),
	mp_rocket(NULL)
{
	m_spriteWidth = 64;
	m_spriteHeight = 64;
	m_spriteClipWidth = 56;
	m_spriteClipHeight = 32;

	// Initialise the player's position.
	float xPos = static_cast<float>(Renderer::GetScreenWidth()
		- m_spriteWidth) / 2.f;
	float yPos = static_cast<float>(Renderer::GetScreenHeight()
		- PLAYER_Y_OFFSET - m_spriteHeight);
	m_position = Vector2(xPos, yPos);

	// Get the sprite for the player.
	mp_sprite = Game::GetInstance().GetResourceManager().GetPlayerSprite();

	// Create our rocket.
	m_offscreen = Vector2(static_cast<float>(OFFSCREEN_X), 
						  static_cast<float>(OFFSCREEN_Y));
	mp_rocket = new Rocket(m_offscreen);
}
Player::~Player()
{
}

bool Player::BroadPhase(const RenderableBase& objectOne, 
						const RenderableBase& objectTwo)
{
	Vector2 onePos = objectOne.GetPosition();
	Vector2 twoPos = objectTwo.GetPosition();
	
	// We use a base of 64 pixels for the broad phase collision
	// unless either of the objects has a larger clip dimension.
	int overlap = 64;
	overlap = max( objectOne.GetClipWidth(), overlap );
	overlap = max( objectOne.GetClipWidth(), overlap );
	overlap = max( objectTwo.GetClipHeight(), overlap );
	overlap = max( objectTwo.GetClipHeight(), overlap );

	if ( abs(onePos.x - twoPos.x) < overlap )
	{
		if ( abs(onePos.y - twoPos.y) < overlap )
		{
#ifdef _DEBUG
			const_cast<RenderableBase&>(objectOne).DEBUG_SetColliding(true);
			const_cast<RenderableBase&>(objectTwo).DEBUG_SetColliding(true);
#endif

			return true;
		}
	}

#ifdef _DEBUG
	const_cast<RenderableBase&>(objectOne).DEBUG_SetColliding(false);
	const_cast<RenderableBase&>(objectTwo).DEBUG_SetColliding(false);
#endif

	return false;
}

Vector2 Player::GetCollisionMidpoint(const RenderableBase& object)
{
	Vector2 currentPos = object.GetPosition();

	float midX =
	    currentPos.x + (object.GetSpriteWidth() / 2.0f);
	float midY =
	    currentPos.y + (object.GetSpriteHeight() / 2.0f);

	return Vector2(midX, midY);
}

bool Player::NarrowPhase(const RenderableBase& objectOne,
						 const RenderableBase& objectTwo)
{
	// Get the centers of each object.
	Vector2 midOne = GetCollisionMidpoint(objectOne);
	Vector2 midTwo = GetCollisionMidpoint(objectTwo);

	int totalWidth = ( objectOne.GetClipWidth() + objectTwo.GetClipWidth() ) / 2;
	int totalHeight = ( objectOne.GetClipHeight() + objectTwo.GetClipHeight() ) / 2;

	if ( abs(midOne.x - midTwo.x) < totalWidth )
	{
		if ( abs(midOne.y - midTwo.y) < totalHeight )
		{
			return true;
		}
	}

	return false;
}

bool Player::CheckCollision(const RenderableBase& objectOne,
							const RenderableBase& objectTwo)
{
	// Check if the two objects are within the same 32x32 grid.
	if ( BroadPhase(objectOne, objectTwo) )
	{
		// If they are, check if the actual clip rectangles (that is,
		// the non-black part of the sprite) are intersecting.
		if ( NarrowPhase(objectOne, objectTwo) )
		{
			return true;
		}
	}

	return false;
}

void Player::Update(float frameTime)
{
	// Manage collisions between the player and enemy projectiles, and between 
	// the player rocket and enemies.

	// Get a local enemy list.
	const std::list<Enemy*>& enemyList = 
		Game::GetInstance().GetEnemyManager().GetEnemyList();

	if ( mp_rocket != NULL )
	{
		std::list<Enemy*>::const_iterator it = enemyList.begin();
		for ( it; it != enemyList.end(); ++it )
		{
			Enemy* enemy = static_cast<Enemy*>(*it);

			// Check for collisions against each individual enemy.
			// Note that we don't need to check if a rocket exists, as
			// this is handled in the collision function.
			if ( CheckCollision(*mp_rocket, *enemy) )
			{
				bool mainGame = GameState::GetInstance().InMainGameMode();
				bool gettingAngry = enemy->IsGettingAngry();

				// We don't want to blow the enemy up if it's clamouring
				// for an education.
				if ( mainGame && gettingAngry )
				{
					// Transition into education mode with this enemy.
					GameState::GetInstance().StartEducation(enemy);

					// Remove the "rocket".
					KillRocket();
				}
				else 
				{
					int score = enemy->GetScore();
					Vector2 pos = enemy->GetPosition();
					Game::GetInstance().GetRenderer().AddScoreText(score, pos);

					// Work out the new difficulty because of the death.
					GameState::GetInstance().RecalculateDifficulty();

					KillRocket();
					enemy->Kill();
				}
			}
		}
	}

	// Get a local projectile list.
	const std::list<ProjectileBase*>& projectileList 
		= Game::GetInstance().GetProjectileManager().GetProjectileList();

	std::list<ProjectileBase*>::const_iterator projIt = projectileList.begin();
	for ( projIt; projIt != projectileList.end(); ++projIt )
	{
		// We only want to examine enemy projectiles.
		if ( (*projIt)->IsEnemyProjectile() )
		{
			EnemyProjectile* p_proj = static_cast<EnemyProjectile*>(*projIt);

			// If the EnemyProjectile has a word, we use the sprite for that
			// for collision instead.
			RenderableBase* obj = 
				( p_proj->GetWord() != NULL )
				? static_cast<RenderableBase*>(p_proj->GetWord())
				: static_cast<RenderableBase*>(p_proj);

			if ( CheckCollision(*this, *obj) )
			{
				// We've been hit!
				m_health--;

				// Make a loud noise to indicate the player being hit.
				Game::GetInstance().GetAudioManager().PlaySoundEffect("hurt", 255);

				// Destroy the offending projectile.
				p_proj->Kill();
			}
		}
	}
	
	// Check to see if the player has died.
	if ( m_health <= 0 )
	{
		// GAME OVER MAN.. GAME OVER!
		GameState::GetInstance().SetGameOver();
	}
}

void Player::Move(MoveDirection direction, float elapsedTime)
{
	// Convert elapsed time to seconds (from ms)
	elapsedTime /= 1000.0f;
	float offset = elapsedTime * PLAYER_VELOCITY;

	offset *= direction;

	// Update the player's position
	m_position.x += offset;

	// Ensure we don't go out of bounds
	BoundMovement();
}

void Player::BoundMovement()
{
	float rightBounds =
		static_cast<float>(
			Renderer::GetScreenWidth() - 
			m_spriteWidth
		);
	MathsHelper::Clamp(m_position.x, 0.0f, rightBounds);
}

void Player::KillRocket()
{
	// Kill the rocket ...
	mp_rocket->Kill();

	// ... and move it off-screen.
	mp_rocket->SetPosition(m_offscreen);
}

void Player::Fire()
{
	// We only fire if we don't already have a rocket in motion.
	Vector2 rocketPos = mp_rocket->GetPosition();
	// Check if we're off screen. If we are, we can fire.
	if ( rocketPos.x < 0 && rocketPos.y < 0 )
	{
		// Make the Rocket live again ...
		mp_rocket->Resurrect();

		// ... and move it back onto the screen.
		Vector2 pos = m_position;
		pos.x += (m_spriteWidth - mp_rocket->GetSpriteWidth() ) / 2;
		mp_rocket->SetPosition(pos);

		// Add the rocket back into list in the ProjectileManager.
		Game::GetInstance().GetProjectileManager().SpawnProjectile(*mp_rocket);

		// Play the shooting sound effect.
		Game::GetInstance().GetAudioManager().PlaySoundEffect("playershoot");
	}
}

void Player::Render()
{
	// Use the Renderer to draw the player's sprite.
	Game::GetInstance().GetRenderer().DrawSprite(
		mp_sprite, m_position.x, m_position.y, m_spriteWidth, m_spriteHeight
	);

#ifdef _DEBUG
	Vector2 boundPos = m_position;
	boundPos.x -= (32 - (m_spriteWidth/2));
	boundPos.y -= (32 - (m_spriteHeight/2));

	Game::GetInstance().GetRenderer().DEBUG_DrawBox(
		boundPos.x, boundPos.y, 64, 64, 
		Renderer::GetColour(255, 255, 255)
	);
#endif
}
