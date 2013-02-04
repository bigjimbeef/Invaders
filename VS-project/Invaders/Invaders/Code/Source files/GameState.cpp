#include "GameState.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

GameState::GameState() :
	m_paused(false),
	m_playerScore(0),
	m_difficulty(0),
	m_gameOver(false),
	m_transitioningToEducation(false),
	m_transitioningFromEducation(false),
	mp_tutee(NULL),
	m_inMainGameMode(false),
	m_inEducationMode(false),
	m_timeEducating(0.0f),
	m_waveNumber(0)
{
	
}
GameState::~GameState()
{
}

void GameState::IncrementWaveNumber()
{ 
	m_waveNumber++;

	if ( m_waveNumber > MAX_WAVES ) 
	{ 
		m_waveNumber = 0; 
	}

	// Set the base difficulty for this wave.
	m_difficulty = DIFFICULTY_PER_WAVE * m_waveNumber;
}

void GameState::RecalculateDifficulty()
{
	// Get the number of remaining enemies, and change our difficulty based
	// on that.
	int remaining = Game::GetInstance().GetEnemyManager().GetRemainingEnemies();
	int total = Game::GetInstance().GetEnemyManager().GetTotalEnemies();
	float remainingF = static_cast<float>(remaining);
	float totalF = static_cast<float>(total);

	float proportion = 1.0f - (remainingF / totalF);
	int difficulty = static_cast<int>(proportion * MAX_DIFFICULTY_PER_WAVE);

	int newDifficulty = (DIFFICULTY_PER_WAVE * m_waveNumber) + difficulty;
	newDifficulty = min(newDifficulty, MAX_WAVES * MAX_DIFFICULTY_PER_WAVE);

	m_difficulty = newDifficulty;
}

void GameState::TransitionToEducation()
{
	if ( m_transitioningToEducation )
	{
		float target = 0.1f;
		float speed = Game::GetInstance().GetSpeedFactor();
		float diff = speed - target;

		MathsHelper::Lerp(speed, target, 0.025f);
		Game::GetInstance().SetSpeedFactor(speed);

		if ( MathsHelper::Abs(diff) < 0.1f ) {
			m_transitioningToEducation = false;
			m_inEducationMode = true;

			// Set-up the word to educate with.
			StartTeaching();
		}
	}
}
void GameState::TransitionFromEducation()
{
	m_inEducationMode = false;

	if ( m_transitioningFromEducation )
	{
		float target = 1.0f;
		float speed = Game::GetInstance().GetSpeedFactor();
		float diff = speed - target;

		MathsHelper::Lerp(speed, target, 0.025f);
		Game::GetInstance().SetSpeedFactor(speed);

		if ( MathsHelper::Abs(diff) < 0.1f ) {
			// Lock back to 100% play speed.
			Game::GetInstance().SetSpeedFactor(target);

			m_transitioningFromEducation = false;
		}
	}
}

void GameState::StartEducation(Enemy* tutee)
{
	mp_tutee = tutee;
	m_transitioningFromEducation = false;
	m_transitioningToEducation = true;
}

void GameState::EndEducation()
{
	m_timeEducating = 0.0f;
	mp_tutee = NULL;
	m_transitioningFromEducation = true;
}

bool GameState::ShouldTransitionToMainGameMode()
{
	// We transition when we have either:
	// 10 kills.
	int remaining = Game::GetInstance().GetEnemyManager().GetRemainingEnemies();
	int total = Game::GetInstance().GetEnemyManager().GetTotalEnemies();
	if ( total - remaining >= KILLS_TO_CHANGE )
	{
		return true;
	}

	// 30 seconds have passed.
	float timePassed = Game::GetInstance().GetTotalTime() / 1000.0f;
	if ( timePassed >= SECONDS_TO_CHANGE )
	{
		return true;
	}

	// The player has taken two damage.
	int playerHealthTotal = Game::GetInstance().GetPlayer().GetTotalHealth();
	int playerHealthNow = Game::GetInstance().GetPlayer().GetCurrentHealth();
	if ( playerHealthTotal - playerHealthNow >= DAMAGE_TO_CHANGE )
	{
		return true;
	}

	return false;
}

void GameState::TransitionToMainGameMode(float frameTime)
{
	Game::GetInstance().GetInputController().SetControlsBlocked(true);

	// Do the transition!
}

void GameState::Update(float frameTime)
{
	// Check to see if we need to move to the second game mode.
	if ( !m_inMainGameMode )
	{
		if ( ShouldTransitionToMainGameMode() )
		{
			TransitionToMainGameMode(frameTime);
		}
	}
	else
	{
		if ( m_transitioningToEducation )
		{
			TransitionToEducation();
		}
		else if ( m_transitioningFromEducation )
		{
			TransitionFromEducation();
		}

		if ( m_inEducationMode )
		{
			// Increase the timer.
			m_timeEducating += ( frameTime / 1000.0f );

			float targetTime = static_cast<float>(BASE_EDUCATION_TIME);
			if ( m_timeEducating > targetTime )
			{
				// End the education mode.
				EndEducation();

				// Fire a massive letter.
				Game::GetInstance().GetEnemyManager().FireMammothLetter();

				// Destroy the word.
				Game::GetInstance().GetEnemyManager().RemoveWord();
			}
		}
	}
}

void GameState::StartTeaching()
{
	// Ensure we don't try to teach the aether.
	if ( mp_tutee != NULL )
	{
		mp_tutee->GenerateWord();
	}
}

void GameState::IncTimeEducating(float value)
{
	m_timeEducating += value;
	float baseTime = static_cast<float>(BASE_EDUCATION_TIME);
	MathsHelper::Ceil(m_timeEducating, baseTime);
}

void GameState::Render()
{
#ifdef _DEBUG
	std::stringstream ss;
	int score = GameState::GetInstance().GetDifficulty();
	ss << "Difficulty: " << score;
	Game::GetInstance().GetRenderer().DrawText(ss.str(), Vector2());
#endif

	if ( m_paused )
	{
		int semitrans = 200;
		Game::GetInstance().GetRenderer().RenderOverlay(semitrans);

		std::string paused = "Paused. Press Enter to resume.";
		RECT fontDimensions = Game::GetInstance().GetRenderer().MeasureString(paused, true);
		int width = fontDimensions.right - fontDimensions.left;
		int height = fontDimensions.bottom - fontDimensions.top;

		float xPos = static_cast<float>( Renderer::GetScreenWidth() - width ) / 2.0f;
		float yPos = static_cast<float>( Renderer::GetScreenHeight() - height ) / 2.0f;

		Game::GetInstance().GetRenderer().DrawText(paused, Vector2(xPos, yPos), true);
	}
}