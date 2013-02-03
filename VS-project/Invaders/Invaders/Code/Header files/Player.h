#ifndef PLAYER_H
#define PLAYER_H

//-----------------------------------------------------------------------------
// Player.h
// The player is represented in game as the little laser platform at the
// bottom of the screen. This class stores the number of lives the player has,
// as well as handling collision between all objects.
//-----------------------------------------------------------------------------

// Include our base class.
#include "Renderable.h"

#include "Rocket.h"

// Forward declare Game to allow access to Game singleton.
class Game;

enum MoveDirection
{
	MOVE_LEFT = -1,
	MOVE_RIGHT = 1,
};

class Player : public IRenderable
{
	public:
		Player();
		virtual ~Player();

		// Implement this function to show that this class is not abstract.
		inline void IsAbstract() { /* Do nothing */ }

		virtual void Update(float frameTime);
		virtual void Render();

		// Used to the move the player around the screen, with the direction
		// indicating the cardinality of the X move:
		//     1 is right, -1 is left.
		void Move(MoveDirection direction, float elapsedTime);

		// This function ensures we don't go outside the game world.
		void BoundMovement();

		// Controls the player firing a rocket.
		void Fire();

		// Removes the player's rocket.
		inline void KillRocket() { mp_rocket = 0; }

		// Check collision between two Renderable objects.
		bool CheckCollision(const IRenderable& objectOne, const IRenderable& objectTwo);
		Vector2 GetCollisionMidpoint(const IRenderable& object);
		bool BroadPhase(const IRenderable& objectOne, const IRenderable& objectTwo);
		bool NarrowPhase(const IRenderable& objectOne, const IRenderable& objectTwo);

		//---------------------------------------------------------------------
		// Accessors
		inline int GetCurrentHealth() const { return m_health; }
		static int GetTotalHealth() { return STARTING_HEALTH; }

	private:
		// The current health of the player.
		int m_health;
		static const int STARTING_HEALTH = 3;
		static const int PLAYER_VELOCITY = 160;

		// How far is the player from the bottom of the screen?
		static const int PLAYER_Y_OFFSET = 70; 

		// The player is only allowed one rocket at once. This is a pointer
		// to that rocket.
		Rocket* mp_rocket;
};

#endif // PLAYER_H