#include "Player.h"

// Needed for abs
#include <cmath>
#include <list>

// Include within .cpp file to match forward declare in .h
#include "Game.h"

#define SAFE_DELETE(x) if (x) { delete x; x = NULL; }

Player::Player(float xPos, float yPos) :
	m_health(STARTING_HEALTH),
	mp_rocket(NULL)
{
	// Initialise the player's position.
	m_position = Position(xPos, yPos);

	m_spriteClipWidth = 28;
	m_spriteClipHeight = 20;
	m_spriteClipXOffset = 2;
	m_spriteClipYOffset = 7;
}
Player::~Player()
{
	mp_sprite = NULL;
}

void Player::Init()
{
	// Create the sprite for the player.
	mp_sprite = ResourceManager::GetPlayerSprite();
}

bool Player::BroadPhase(Position& one, Position& two, const int bounds)
{
	if ( abs(one.x - two.x) < bounds )
	{
		if ( abs(one.y - two.y) < bounds )
		{
			return true;
		}
	}

	return false;
}

Position Player::GetCollisionMidpoint(IRenderable& object)
{
	Position currentPos = object.GetPosition();

	float midX =
	    currentPos.x + (object.GetXOffset() + (object.GetWidth() / 2.0f));
	float midY =
	    currentPos.y + (object.GetYOffset() + (object.GetHeight() / 2.0f));

	return Position(midX, midY);
}

bool Player::NarrowPhase(IRenderable& objectOne, IRenderable& objectTwo)
{
	// Get the centers of each object.
	Position midOne = GetCollisionMidpoint(objectOne);
	Position midTwo = GetCollisionMidpoint(objectTwo);

	int totalWidth = ( objectOne.GetWidth() + objectTwo.GetWidth() ) / 2;
	int totalHeight = ( objectOne.GetHeight() + objectTwo.GetHeight() ) / 2;

	if ( abs(midOne.x - midTwo.x) < totalWidth )
	{
		if ( abs(midOne.y - midTwo.y) < totalHeight )
		{
			return true;
		}
	}

	return false;
}

bool Player::CheckCollision(IRenderable& objectOne, IRenderable& objectTwo)
{
	if ( &objectOne == NULL || &objectTwo == NULL )
	{
		return false;
	}

	Position onePos = objectOne.GetPosition();
	Position twoPos = objectTwo.GetPosition();
	
	// This is constant for all sprites currently.
	int bounds = Game::GetSpriteSize();

	// Check if the two objects are within the same 32x32 grid.
	if ( BroadPhase(onePos, twoPos, bounds) )
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

void Player::Update(const float frameTime)
{
	// Manage collisions between the player and bombs, and between the player
	// rocket and enemies.

	// Get a local enemy list.
	const std::list<Enemy*>& enemyList = 
		EnemyManager::GetInstance().GetEnemyList();

	std::list<Enemy*>::const_iterator it = enemyList.begin();
	for ( it; it != enemyList.end(); ++it )
	{
		Enemy* enemy = static_cast<Enemy*>(*it);

		// Check for collisions against each individual enemy.
		// Note that we don't need to check if a rocket exists, as
		// this is handled in the collision function.
		if ( CheckCollision(*mp_rocket, *enemy) )
		{
			mp_rocket->Kill();
			enemy->Kill();
		}
	}

	// Get a local projectile list.
	const std::list<IProjectile*>& projectileList 
		= ProjectileManager::GetInstance().GetProjectileList();

	std::list<IProjectile*>::const_iterator projIt = projectileList.begin();
	for ( projIt; projIt != projectileList.end(); ++projIt )
	{
		// We only want to examine bombs.
		if ( (*projIt)->IsBomb() )
		{
			Bomb* p_bomb = static_cast<Bomb*>(*projIt);

			if ( CheckCollision(*this, *p_bomb) )
			{
				// We've been hit!
				m_health--;

				// Destroy the offending bomb.
				p_bomb->Kill();
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

void Player::Move(const int direction, const float elapsedTime)
{
	float offset = elapsedTime * PLAYER_VELOCITY;

	if ( direction == -1 )
	{
		// Moving to the left.
		offset = -offset;
	}

	// Update the player's position
	m_position.x += offset;

	// Ensure we don't go out of bounds
	BoundMovement();
}

void Player::BoundMovement()
{
	m_position.x = max(0.0f, m_position.x);
	float rightBounds =
		static_cast<float>(
			Game::GetScreenWidth() - 
			Game::GetSpriteSize()
		);

	m_position.x = min(m_position.x, rightBounds);
}

void Player::Fire()
{
	// We only fire if we don't already have a rocket in motion.
	if ( mp_rocket == NULL )
	{
		// Create a new rocket ...
		mp_rocket = new Rocket();
		// ... and spawn it with the ProjectileManager.
		ProjectileManager::GetInstance().SpawnProjectile(*mp_rocket);
	}
}

void Player::Render()
{
	// Draw the sprite for this renderable item.
	mp_sprite->draw(static_cast<int>(m_position.x),
					static_cast<int>(m_position.y));
}
