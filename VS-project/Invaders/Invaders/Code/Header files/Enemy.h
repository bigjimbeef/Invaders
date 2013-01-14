#ifndef ENEMY_H
#define ENEMY_H

// Include our base class.
#include "Renderable.h"

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

		void Update(float frameTime, int direction, bool dropDown);
		virtual void Render();

		// Moves the enemy around the screen.
		void Move(int direction, float elapsedTime, bool dropDown);

		// Controls the enemy firing a bomb.
		// void Fire();

		// Removes one of the enemy's bombs.
		// inline void KillBomb() { mp_rocket = 0; }

		void Kill();

		//---------------------------------------------------------------------
		// Accessors
		inline bool IsAlive() { return m_alive; }
		
		inline int GetRow() { return m_row; }
		inline int GetCol() { return m_col; }

	private:
		// Is this enemy alive?
		bool m_alive;

		// The starting row for the enemy. This controls their look.
		int m_row;
		// The starting col for the enemy. This is used to calculate movement.
		int m_col;

		// The score that the player will be awarded for killing this enemy.
		int m_score;

		// This handles basic animation of sprites.
		bool m_altSprite;

		// Each enemy is allowed three bombs at once. This array represents
		// the bombs.
		// Bomb* mp_bombs[3];

};

#endif // ENEMY_H