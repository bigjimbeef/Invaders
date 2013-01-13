#include "EnemyManager.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

EnemyManager::EnemyManager() :
	m_remainingCols(NUM_COLS),
	m_pauseDuration(0.0f),
	m_basePauseDuration(1.0f),
	m_currentPause(0.0f)
{
	m_pauseDuration = m_basePauseDuration;

	// Create a new vector for holding enemies.
	m_enemies = std::list<Enemy*>();
}
EnemyManager::~EnemyManager()
{
	// We loop through the projectile list and ensure that we correctly
	// delete each item in it.
	while (!m_enemies.empty()) {
		delete m_enemies.back();
		m_enemies.pop_back();
	}

	m_enemies.clear();
}

void EnemyManager::Update(float frameTime)
{
	// Increment the timer.
	m_currentPause += frameTime;

	// We time the enemy movement so that they move to a rhythm.
	if ( m_currentPause > m_pauseDuration )
	{
		// We need to calculate some information to pass to each Enemy before
		// we update them.
		// i.e. We need to know which way to travel, and whether to drop down

		std::list<Enemy*>::iterator it = m_enemies.begin();
		for ( it = m_enemies.begin(); it != m_enemies.end(); )
		{
			// Update this individual enemy.
			Enemy* enemy = static_cast<Enemy*>(*it);
			enemy->Update(frameTime);
		
			if ( !enemy->IsAlive() )
			{
				// If that is the case, we must remove and delete it from the list.
				delete *it;
				*it = 0;
				it = m_enemies.erase(it);
			}
			else
			{
				++it;
			}
		}

		// Reset the timer.
		m_currentPause = 0.0f;
	}
}

void EnemyManager::Render()
{
	std::list<Enemy*>::iterator it = m_enemies.begin();
	for ( it; it != m_enemies.end(); ++it )
	{
		// Render the enemy.
		Enemy* enemy = static_cast<Enemy*>(*it);
		if ( enemy->IsAlive() )
		{
			enemy->Render();
		}
	}
}

void EnemyManager::SpawnWave()
{
	// Enemies are spawned from the top-left initially. This is offset
	// by a single row based on the wave number.
	int baseXPosition = 0;
	int baseYPosition = 0 + ( GameState::GetInstance().GetWaveNumber() * ROW_OFFSET );

	// We need to find out how many enemies are in the back two rows, as these
	// are worth more points.
	int backTwoRowCount = ( NUM_COLS * 2 );
	int totalNumEnemies = NUM_ROWS * NUM_COLS;
	
	for ( int i = 0; i < totalNumEnemies; ++i )
	{
		// The back two rows of enemies are worth more points.
		int score = ( i < backTwoRowCount ) ? BACK_ENEMY_SCORE : FRONT_ENEMY_SCORE;

		// As we are using a 1D list, we need to calculate row and col number.
		int currentCol = i % NUM_COLS;
		int currentRow = i % NUM_ROWS;

		float xPos = static_cast<float>(baseXPosition + ( COL_OFFSET * currentCol ));
		float yPos = static_cast<float>(baseYPosition + ( ROW_OFFSET * currentRow ));

		// Create a new enemy, then spawn it.
		Enemy* p_baddie = new Enemy(xPos, yPos, currentRow, currentCol, score);
		p_baddie->Init();
		
		m_enemies.push_back(p_baddie);
	}

	// We have a complete field.
	m_remainingCols = NUM_COLS;
}

void EnemyManager::CalculateNewColWidth()
{
	int max = 0;

	std::list<Enemy*>::iterator it = m_enemies.begin();
	for ( it; it != m_enemies.end(); ++it )
	{
		Enemy* enemy = static_cast<Enemy*>(*it);
		if ( enemy->IsAlive() )
		{
			if ( enemy->GetCol() > max )
			{
				max = enemy->GetCol();
			}
		}
	}

	// We now cache the new number of cols remaining.
	m_remainingCols = max;
}