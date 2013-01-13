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
		Enemy(float xPos, float yPos, int row, int score);
		~Enemy();

		// Implement this function to show that this class is not abstract.
		inline void IsAbstract() { /* Do nothing */ }

		// Initialises the enemy.
		void Init();

		void Update(float frameTime);

		// Moves the enemy around the screen.
		void Move(int direction, float elapsedTime);

		// Controls the enemy firing a bomb.
		// void Fire();

		// Removes one of the enemy's bombs.
		// inline void KillBomb() { mp_rocket = 0; }

		//---------------------------------------------------------------------
		// Accessors
		inline bool IsAlive() { return m_alive; }

	private:
		// Is this enemy alive?
		bool m_alive;

		// The starting row for the enemy. This controls their look.
		int m_row;

		// The score that the player will be awarded for killing this enemy.
		int m_score;

		// Each enemy is allowed three bombs at once. This array represents
		// the bombs.
		// Bomb* mp_bombs[3];

};

#endif // ENEMY_H