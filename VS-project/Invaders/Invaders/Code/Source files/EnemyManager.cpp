#include "EnemyManager.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

EnemyManager::EnemyManager() :
	m_maxCol(NUM_COLS - 1), // Used in zero-indexed array
	m_minCol(0),
	m_directionOfTravel(1),
	m_currentX(0.0f),
	m_remainingEnemies(0),
	m_pauseDuration(0.0f),
	m_basePauseDuration(0.75f),
	m_currentPause(0.0f),
	m_respawnTime(1.0f),
	m_minStep(0.01f),
	m_stepReductionPerKill(0.0f)
{
	m_pauseDuration = m_basePauseDuration;

	// Create a new vector for holding enemies.
	m_enemies = std::list<Enemy*>();

	// Calculate the step reduction per kill.
	m_stepReductionPerKill = 
		( m_basePauseDuration - m_minStep ) / (NUM_COLS * NUM_ROWS );
}
EnemyManager::~EnemyManager()
{
	ClearEnemyList();
}

void EnemyManager::ClearEnemyList()
{
	// We loop through the enemy list and ensure that we correctly
	// delete each item in it.
	while (!m_enemies.empty()) {
		delete m_enemies.back();
		m_enemies.pop_back();
	}

	m_enemies.clear();
}

bool EnemyManager::CheckForDrop()
{
	if ( m_directionOfTravel > 0 )
	{
		// If we're moving to the right, we want to know when the right of the
		// pack has touched the right hand edge.
		// To calculate the right of the pack, we use the col width for each
		// but the last column, where we use the sprite width.
		int packWidth = (COL_OFFSET * ( m_maxCol - m_minCol )) + SPRITE_WIDTH;
		float packRight = m_currentX + static_cast<float>(packWidth);

		float screenRight = 
			static_cast<float>( Game::GetInstance().GetScreenWidth() );

		if ( packRight >= screenRight )
		{
			m_directionOfTravel *= -1;
			return true;
		}
	}
	else
	{
		// When going left, we check when the edge of the pack is off the left
		// side of the screen.
		if ( ( m_currentX - MOVE_DISTANCE ) <= 0.0f )
		{
			m_directionOfTravel *= -1;
			return true;
		}
	}

	return false;
}

void EnemyManager::UpdateMovementSpeed()
{
	int totalNumEnemies = NUM_ROWS * NUM_COLS;

	// The duration of the pause is inversely proportional to the number
	// of enemies remaining; capped at MIN_STEP.
	// duration = base_duration - ( ( totalEn - currenEn ) * eachStep
	float duration = m_basePauseDuration -
		( totalNumEnemies - m_remainingEnemies ) * m_stepReductionPerKill;

	// Now actually utilise this value.
	m_pauseDuration = duration;
}

void EnemyManager::Update(float frameTime)
{
	// Increment the timer.
	m_currentPause += frameTime;

	// Check if the enemy list is empty and we need to spawn a new wave.
	if ( m_enemies.size() <= 0 )
	{
		if ( m_currentPause > m_respawnTime )
		{
			GameState::GetInstance().IncrementWaveNumber();
			
			// Spawn a new wave.
			SpawnWave();

			// Reset the timer.
			m_currentPause = 0.0f;
		}

		return;
	}

	// We time the enemy movement so that they move to a rhythm.
	if ( m_currentPause > m_pauseDuration )
	{
		// We need to calculate some information to pass to each Enemy before
		// we update them.
		// i.e. We need to know which way to travel, and whether to drop down
		bool shouldDrop = CheckForDrop();

		std::list<Enemy*>::iterator it = m_enemies.begin();
		for ( it = m_enemies.begin(); it != m_enemies.end(); )
		{
			// Update this individual enemy.
			Enemy* enemy = static_cast<Enemy*>(*it);
			enemy->Update(frameTime, m_directionOfTravel, shouldDrop);
		
			if ( !enemy->IsAlive() )
			{
				// If that is the case, we remove and delete it from the list.
				delete *it;
				*it = 0;
				it = m_enemies.erase(it);
				
				// Decrement the number of enemies remaining.
				m_remainingEnemies--;
			}
			else
			{
				++it;
			}
		}

		// Modulate the movement speed of remaining enemies based on the number
		// of enemies that are remaining.
		UpdateMovementSpeed();

		// Update the X position.
		m_currentX += ( m_directionOfTravel * MOVE_DISTANCE );

		// Reset the timer.
		m_currentPause = 0.0f;
	}

#ifdef _DEBUG
	std::stringstream ss;
	ss << "Min: " << m_minCol;
	std::string min = ss.str();

	ss.str("");
	ss << "Max: " << m_maxCol;
	std::string max = ss.str();

	Game::GetInstance().GetSystem().drawText(0, 250, min.c_str());
	Game::GetInstance().GetSystem().drawText(0, 270, max.c_str());
#endif
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
	m_currentX = static_cast<float>(baseXPosition);

	int baseYPosition = 
		0 + ( GameState::GetInstance().GetWaveNumber() * ROW_OFFSET );

	// We need to find out how many enemies are in the back two rows, as these
	// are worth more points.
	int backTwoRowCount = ( NUM_COLS * 2 );
	int totalNumEnemies = NUM_ROWS * NUM_COLS;
	
	int currentCol = 0;
	int currentRow = 0;

	for ( int i = 0; i < totalNumEnemies; ++i )
	{
		// The back two rows of enemies are worth more points.
		int score = ( i < backTwoRowCount ) 
			? BACK_ENEMY_SCORE
			: FRONT_ENEMY_SCORE;

		// As we are using a 1D list, we need to calculate row and col number.
		currentCol = i % NUM_COLS;
		currentRow = i / NUM_COLS;

		float xPos = 
			static_cast<float>(baseXPosition + ( COL_OFFSET * currentCol ));
		float yPos = 
			static_cast<float>(baseYPosition + ( ROW_OFFSET * currentRow ));

		// Create a new enemy, then spawn it.
		Enemy* p_baddie = new Enemy(xPos, yPos, currentRow, currentCol, score);
		p_baddie->Init();
		
		m_enemies.push_back(p_baddie);
	}

	// We have a complete field. Note that this is -1 as it is used as an array
	// index.
	m_maxCol = NUM_COLS - 1;
	m_minCol = 0;
	m_remainingEnemies = totalNumEnemies;
	
	// Start moving to the right.
	m_directionOfTravel = 1;
}

void EnemyManager::CalculateNewColWidth()
{
	int max = 0;
	int min = m_maxCol;
	Enemy* p_minEnemy = NULL;

	std::list<Enemy*>::iterator it = m_enemies.begin();
	for ( it; it != m_enemies.end(); ++it )
	{
		Enemy* enemy = static_cast<Enemy*>(*it);
		if ( enemy->IsAlive() )
		{
			// Ensure we get an enemy to look at.
			if ( p_minEnemy == NULL )
			{
				p_minEnemy = enemy;
			}

			if ( enemy->GetCol() > max )
			{
				max = enemy->GetCol();
			}
			if ( enemy->GetCol() < min )
			{
				min = enemy->GetCol();
				p_minEnemy = enemy;
			}
		}
	}

	// If we reach this point without an Enemy*, the list is empty.
	if ( p_minEnemy == NULL )
	{
		return;
	}

	// We now cache the new number of cols remaining.
 	m_maxCol = max;
	m_minCol = min;

	// The furthest left enemy's x position is now the x position of the wave.
	m_currentX = p_minEnemy->GetPosition().x;
}