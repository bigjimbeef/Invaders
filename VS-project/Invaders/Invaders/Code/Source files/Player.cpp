#include "Player.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

#define SAFE_DELETE(x) if (x) { delete x; x = NULL; }

Player::Player(float xPos, float yPos) :
	m_health(STARTING_HEALTH),
	mp_rocket(0)
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

bool Player::BroadPhase(Position& one, Position& two, int bounds)
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

	float midX = currentPos.x + ( object.GetXOffset() + ( object.GetWidth() / 2.0f ) );
	float midY = currentPos.y + ( object.GetYOffset() + ( object.GetHeight() / 2.0f ) );

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

bool Player::CheckCollision(IRenderable* objectOne, IRenderable* objectTwo)
{
	if ( objectOne == 0 || objectTwo == 0 )
	{
		return false;
	}

	Position onePos = objectOne->GetPosition();
	Position twoPos = objectTwo->GetPosition();
	
	// This is constant for all sprites currently.
	int bounds = objectOne->GetSpriteBounds();

	// Check if the two objects are within the same 32x32 grid.
	if ( BroadPhase(onePos, twoPos, bounds) )
	{
		// If they are, check if the actual clip rectangles (that is,
		// the non-black part of the sprite) are intersecting.
		if ( NarrowPhase(*objectOne, *objectTwo) )
		{
			return true;
		}
	}

	return false;
}

void Player::Update(float frameTime)
{
	// Manage collisions between the player and enemies, and between the player
	// rocket and enemies.

	// Get a local enemy list.
	std::list<Enemy*> enemyList = EnemyManager::GetInstance().GetEnemyList();

	std::list<Enemy*>::iterator it = enemyList.begin();
	for ( it; it != enemyList.end(); ++it )
	{
		// Check for collisions against each individual enemy.
		// Note that we don't need to check if a rocket exists, as
		// this is handled in the collision function.
		if ( CheckCollision(mp_rocket, *it) )
		{
			mp_rocket->Kill();
			
			Enemy* enemy = static_cast<Enemy*>(*it);
			enemy->Kill();
		}
		// We don't need to check if the player collides if the rocket did.
		else if ( CheckCollision(this, *it) )
		{
			// TODO: Player collision!
		} 
	}
	
	// TODO: Update player's health.
}

void Player::Move(int direction, float elapsedTime)
{
	float offset = elapsedTime * PLAYER_VELOCITY;

	if ( direction == -1 )
	{
		// Moving to the left.
		offset = -offset;
	}

	// Update the player's position
	m_position.x += offset;
}

void Player::Fire()
{
	// We only fire if we don't already have a rocket in motion.
	if ( mp_rocket == 0 )
	{
		// Create a new rocket ...
		mp_rocket = new Rocket();
		// ... and spawn it with the ProjectileManager.
		ProjectileManager::GetInstance().SpawnProjectile(mp_rocket);
	}
}

void Player::Render()
{
	// Draw the sprite for this renderable item.
	mp_sprite->draw(int(m_position.x), int(m_position.y));
}