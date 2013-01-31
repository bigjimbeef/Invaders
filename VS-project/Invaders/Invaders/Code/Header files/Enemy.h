#ifndef ENEMY_H
#define ENEMY_H

//-----------------------------------------------------------------------------
// Enemy.h
// Represents one the enemies that are part of the Invaders' armada. They come
// in two forms based on their position in the group of Invaders.
//-----------------------------------------------------------------------------

// Uses STL list for managing projectiles.
#include <list>

// Include our base class.
#include "Renderable.h"

#include "Bomb.h"

// Forward declare Game to allow access to Game singleton.
class Game;

class Enemy : public IRenderable
{
	public:
		// Default ctor.
		Enemy(float xPos, float yPos, int row, int col, int score);
		virtual ~Enemy();

		// Implement this function to show that this class is not abstract.
		inline void IsAbstract() { /* Do nothing */ }

		virtual void Update(float frameTime);
		virtual void Render();

		// Moves the enemy around the screen.
		void Move(float distance, bool dropDown);

		// Causes the enemy to jitter in place when they're ready to fire.
		void Jitter(float frameTime);

		// Controls the enemy firing a bomb.
		void Fire();

		// Removes one of the enemy's bombs.
		void KillBomb(const Bomb& dyingBomb);

		// Kill the enemy. This needs to update more than the bool below, so it
		// is declared in the cpp.
		void Kill();

		//---------------------------------------------------------------------
		// Accessors
		inline bool IsAlive() const { return m_alive; }

		inline int GetRow() const { return m_row; }
		inline int GetCol() const { return m_col; }

		inline void EnableWeapon() { m_canFire = true; }
		
		// Check if we have any bombs currently alive.
		inline bool HasLiveBomb() const { return (m_bombs.size() > 0); }

	private:
		// Is this enemy alive?
		bool m_alive;

		// Can this enemy fire?
		bool m_canFire;

		bool m_gettingAngry;
		int m_anger;
		Vector2 m_currentJitter;
		float m_jitterTimer;
		float m_baseJitterWait;
		static const int MAX_JITTER_OFFSET = 5;
		static const int ANGER_THRESHOLD = 1;
		static const int MAX_ANGER = 100;

		// The starting row for the enemy. This controls their look.
		int m_row;
		// The starting col for the enemy. This is used to calculate movement.
		int m_col;
		// How far has the enemy dropped.
		float m_dropDistance;

		// The score that the player will be awarded for killing this enemy.
		int m_score;

		// This handles basic animation of sprites.
		bool m_altSprite;

		// Each enemy is allowed a static number of bombs at once. This array
		// holds the bombs.
		std::list<Bomb*> m_bombs;
        static const int MAX_BOMBS = 3;
};

#endif // ENEMY_H
