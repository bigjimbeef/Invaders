#include "EnemyManager.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

EnemyManager::EnemyManager() :
	m_maxCol(NUM_COLS - 1), // Used in zero-indexed array
	m_minCol(0),
	m_directionOfTravel(MOVE_RIGHT),
	m_currentX(0.0f),
	m_lowestPoint(0.0f),
	m_enemyProgress(0.0f),
	m_remainingEnemies(0),
	m_totalEnemies(0),
	m_currentPause(0.0f),
	m_respawnTime(1.0f),
	m_speedUp(1.0f)
{
	// Create a new vector for holding enemies.
	m_enemies = std::list<Enemy*>();
	m_deadEnemies = std::list<Enemy*>();
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

bool EnemyManager::CheckForDrop(float moveDistance)
{
	if ( m_directionOfTravel == MOVE_RIGHT )
	{
		// If we're moving to the right, we want to know when the right of the
		// pack has touched the right hand edge.
		// To calculate the right of the pack, we use the col width for each
		// but the last column, where we use the sprite width.
		int packWidth = 
			(COL_OFFSET * ( m_maxCol - m_minCol )) + ENEMY_SPRITE_WIDTH;

		float packRight = m_currentX + static_cast<float>(packWidth);

		float screenRight = static_cast<float>( Renderer::GetScreenWidth() );

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
		if ( ( m_currentX - moveDistance ) <= 0.0f )
		{
			m_directionOfTravel *= -1;
			return true;
		}
	}

	return false;
}

void EnemyManager::UpdateMovementSpeed()
{
	// The speed is inversely proportional to the number of enemies remaining.
	float proportionDead = 
		1.0f - (static_cast<float>(m_remainingEnemies) / 
		static_cast<float>(m_totalEnemies));

	m_speedUp = 
		static_cast<float>(BASE_SPEED + (proportionDead * SPEED_UP_AMOUNT));
}

void EnemyManager::Update(float frameTime)
{
	// Increment the timer.
	m_currentPause += ( frameTime / 1000.0f );

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

	// Delete remaining dead enemies before processing the live ones.
	HandleEnemyDeletion();

	// Update the enemies, which updates when they can fire.
	std::list<Enemy*>::iterator it = m_enemies.begin();
	for ( it = m_enemies.begin(); it != m_enemies.end(); )
	{
		// Update this individual enemy ...
		Enemy* enemy = static_cast<Enemy*>(*it);
		// ... if it's alive.
		if ( enemy->IsAlive() )
		{
			enemy->Update(frameTime);

			it++;
		}
		else
		{
			// As Projectiles are stored on Enemies, we can't delete them if 
			// they have any live projectiles currently.
			if ( enemy->HasLiveProjectile() )
			{
				// Add this enemy to the list to be deleted.
				m_deadEnemies.push_back(enemy);
			}
			else
			{
				// If the enemy has no live projectiles, we delete it
				delete *it;
				*it = NULL;
			}
			it = m_enemies.erase(it);
				
			// Decrement the number of enemies remaining.
			m_remainingEnemies--;
		}
	}

	// Calculate the distance we should move this frame.
	frameTime /= 1000.0f;
	float velocity = m_speedUp * ENEMY_VELOCITY;
	float moveDistance = 
		static_cast<float>(m_directionOfTravel * velocity * frameTime);

	// We need to calculate some information to pass to each Enemy before
	// we update them.
	// i.e. We need to know which way to travel, and whether to drop down
	bool shouldDrop = CheckForDrop(moveDistance);

	std::list<Enemy*>::iterator iter = m_enemies.begin();
	for ( iter = m_enemies.begin(); iter != m_enemies.end(); ++iter )
	{
		// Move this individual enemy ...
		Enemy* enemy = static_cast<Enemy*>(*iter);
		// ... if it's alive.
		if ( enemy->IsAlive() )
		{
			enemy->Move(moveDistance, shouldDrop);
		}
	}

	// Modulate the movement speed of remaining enemies based on the number
	// of enemies that are remaining.
	UpdateMovementSpeed();

	// Update the X position with the distance moved this frame.
	m_currentX += ( moveDistance * Game::GetInstance().GetSpeedFactor() );

	// Update the lowest point.
	UpdateLowestPoint();
}

void EnemyManager::UpdateLowestPoint()
{
	// Loop through all the maximum rows, and find the lowest point in any
	// of them.
	for ( int i = 0; i < ( sizeof(m_maximumRows) / sizeof(int) ); ++i )
	{
		float lowest = 
			m_enemyProgress +
			static_cast<float>( m_maximumRows[i] * COL_OFFSET );

		if ( lowest > m_lowestPoint )
		{
			m_lowestPoint = lowest;
		}
	}

	// If they're about to hit the player ...
	float playerY = Game::GetInstance().GetPlayer().GetPosition().y;
	if ( ( playerY - m_lowestPoint ) < DROP_DISTANCE )
	{
		// ... the game is over - INVASION!
		GameState::GetInstance().SetGameOver();
		return;
	}
}

void EnemyManager::HandleEnemyDeletion()
{
	// Now attempt to delete all enemies that have live projectiles.
	if ( m_deadEnemies.size() > 0 )
	{
		std::list<Enemy*>::iterator del = m_deadEnemies.begin();
		for ( del = m_deadEnemies.begin(); del != m_deadEnemies.end(); )
		{
			Enemy* enemy = static_cast<Enemy*>(*del);
			if ( !enemy->HasLiveProjectile() )
			{
				delete *del;
				*del = NULL;

				del = m_deadEnemies.erase(del);
			}
			else
			{
				del++;
			}
		}
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

void EnemyManager::RenderWords()
{
	std::list<Enemy*>::iterator it = m_enemies.begin();
	for ( it; it != m_enemies.end(); ++it )
	{
		// Render the enemy.
		Enemy* enemy = static_cast<Enemy*>(*it);
		if ( enemy->IsAlive() )
		{
			if ( enemy->GetWord() != NULL )
			{
				// Render the word.
				enemy->GetWord()->Render();
			}
		}
	}
}

void EnemyManager::RemoveWord()
{
	std::list<Enemy*>::iterator it = m_enemies.begin();
	for ( it; it != m_enemies.end(); ++it )
	{
		// Render the enemy.
		Enemy* enemy = static_cast<Enemy*>(*it);
		if ( enemy->GetWord() != NULL )
		{
			enemy->RemoveWord();
			break;
		}
	}
}

void EnemyManager::FireMammothLetter()
{
	std::list<Enemy*>::iterator it = m_enemies.begin();
	for ( it; it != m_enemies.end(); ++it )
	{
		// Render the enemy.
		Enemy* enemy = static_cast<Enemy*>(*it);
		if ( enemy->GetWord() != NULL )
		{
			enemy->Fire(true);
			break;
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
		BASE_Y_POS + ( GameState::GetInstance().GetWaveNumber() * ROW_OFFSET );
	m_enemyProgress = 0.0f;

	// We need to find out how many enemies are in the back two rows, as these
	// are worth more points.
	int backTwoRowCount = ( NUM_COLS * 2 );
	int totalNumEnemies = NUM_ROWS * NUM_COLS;
	// Cache that for later.
	m_totalEnemies = totalNumEnemies;
	
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
		Enemy* p_baddie = 
			new Enemy(xPos, yPos, ENEMY_SPRITE_WIDTH, ENEMY_SPRITE_HEIGHT,
					  currentRow, currentCol, score);

		// We can fire if we're in the front row.
		if ( currentRow == NUM_ROWS - 1 )
		{
			p_baddie->EnableWeapon();
			// Initialise the maximum rows array.
			m_maximumRows[currentCol] = currentRow;

			// The lowest point is the bottom of the front row sprites.
			m_lowestPoint = yPos;
		}
		
		m_enemies.push_back(p_baddie);
	}

	// We have a complete field. Note that this is -1 as it is used as an array
	// index.
	m_maxCol = NUM_COLS - 1;
	m_minCol = 0;
	m_remainingEnemies = totalNumEnemies;
	
	// Start moving to the right.
	m_directionOfTravel = MOVE_RIGHT;
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

void EnemyManager::CalculateNewMaxRow(const int col)
{
	// Check for array OOB.
	if ( col < 0 || col >= NUM_COLS )
	{
		return;
	}

	int maxRow = 0;
	Enemy* p_maxEnemy = NULL;

	std::list<Enemy*>::iterator it = m_enemies.begin();
	for ( it; it != m_enemies.end(); ++it )
	{
		Enemy* enemy = static_cast<Enemy*>(*it);
		if ( enemy->IsAlive() )
		{
			if ( enemy->GetCol() == col )
			{
				// >= as there may only be an enemy in row 0 remaining.
				if ( enemy->GetRow() >= maxRow )
				{
					maxRow = enemy->GetRow();
					p_maxEnemy = enemy;
				}
			}
		}
	}

	// Update this index of the max rows array ...
	m_maximumRows[col] = maxRow;

	// ... and if there's an enemy, then ...
	if ( p_maxEnemy != NULL )
	{
		// ... let it know that it's ok to fire.
		p_maxEnemy->EnableWeapon();
	}
}