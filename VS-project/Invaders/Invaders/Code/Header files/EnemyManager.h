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
		inline int GetMaxCol() { return m_maxCol; }
		inline int GetMinCol() { return m_minCol; }

		inline int GetMoveDelta() { return ( m_directionOfTravel * MOVE_DISTANCE ); }
		inline int GetDropDistance() { return DROP_DISTANCE; }

		inline int GetRemainingEnemies() { return m_remainingEnemies; }

	private:

		bool CheckForDrop();

		// This function will update the apparent movement speed of the enemies
		// by reducing the duration between jumps as fewer enemies remain.
		void UpdateMovementSpeed();

		// A list of all enemies in the game world.
		std::list<Enemy*> m_enemies;

		// This integer holds the width of the remaining enemy wave.
		// This is used for calculating when they should alternate direction.
		int m_maxCol;
		// Similarly to the above, but for the left side.		
		int m_minCol;
		int m_directionOfTravel;
		// This is the current X position of the far left of the field.
		float m_currentX;
			
		// Track how many enemies remain.
		int m_remainingEnemies;

		// How long should the enemies pause for before moving?
		// (This decreases as fewer enemies remain)
		float m_pauseDuration;
		float m_basePauseDuration;
		// How long have they been pausing for so far?
		float m_currentPause;
		// Variables for tracking the speed up from killing enemies.
		float m_minStep;
		float m_stepReductionPerKill;

		static const int NUM_ROWS = 5;
		static const int NUM_COLS = 13;
		static const int SPRITE_WIDTH = 32;

		static const int ROW_OFFSET = 40;
		static const int COL_OFFSET = 40;

		static const int MOVE_DISTANCE = 4;
		static const int DROP_DISTANCE = 20;

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