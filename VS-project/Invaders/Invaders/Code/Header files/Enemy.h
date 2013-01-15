#ifndef ENEMY_H
#define ENEMY_H

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

		// Initialises the enemy.
		void Init();

		virtual void Update(const float frameTime);
		virtual void Render();

		// Moves the enemy around the screen.
		void Move(const bool dropDown);

		// Controls the enemy firing a bomb.
		void Fire();

		// Removes one of the enemy's bombs.
		void KillBomb(Bomb& dyingBomb);

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

		// These variables are used to control the randomly-timed firing.
		// See the Fire function for more details.
		float m_bombTimer;
		float m_bombFireTarget;
		static const int MIN_FIRE_TIME = 1;
		static const int MAX_FIRE_TIME = 20;

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
