#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

//-----------------------------------------------------------------------------
// EnemyManager.h
// Creates, updates, and manages the enemies within the game.
//-----------------------------------------------------------------------------

// Uses STL list for managing projectiles.
#include <list>

#include "Enemy.h"

// Forward declare Game to allow access to Game singleton.
class Game;

class EnemyManager
{
	public:

		static EnemyManager& GetInstance()
		{
			static EnemyManager instance;
			return instance;
		}
		virtual ~EnemyManager();

		void Update(float frameTime);

		void Render();

		// Spawn a new wave of enemies.
		void SpawnWave();

		// This function is used when enemies in the outside cols are killed.
		// It ensures the EnemyManager knows the boundaries of the enemy wave.
		void CalculateNewColWidth();

		//---------------------------------------------------------------------
		// Accessors
		inline std::list<Enemy*>& GetEnemyList() { return m_enemies; }
		inline int GetNumRemainingCols() { return m_remainingCols; }

	private:

		// A list of all enemies in the game world.
		std::list<Enemy*> m_enemies;

		// This integer holds the width of the remaining enemy wave.
		// This is used for calculating when they should alternate direction.
		int m_remainingCols;
			
		// How long should the enemies pause for before moving?
		// (This decreases as fewer enemies remain)
		float m_pauseDuration;
		float m_basePauseDuration;
		// How long have they been pausing for so far?
		float m_currentPause;

		static const int NUM_ROWS = 5;
		static const int NUM_COLS = 11;

		static const int ROW_OFFSET = 35;
		static const int COL_OFFSET = 45;

		// TODO: Revisit these scores!
		static const int FRONT_ENEMY_SCORE = 10;
		static const int BACK_ENEMY_SCORE = 30;

		// Private default ctor to facilitate Singleton pattern
		EnemyManager();

		// DO NOT IMPLEMENT
		EnemyManager(EnemyManager const&);
		void operator=(EnemyManager const&);

};

#endif // ENEMYMANAGER_H