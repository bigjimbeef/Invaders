#include "GameState.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

GameState::GameState() :
	m_paused(false),
	m_playerScore(0),
	m_difficulty(0),
	m_gameOver(false),
	m_toMainGame(false),
	m_filePath("Code/Resource files/greeting.txt"),
	m_explanationString(""),
	m_currentExplanation(""),
	m_textTimer(0.0f),
	m_transitioningToEducation(false),
	m_transitioningFromEducation(false),
	mp_tutee(NULL),
	m_inMainGameMode(true),
	m_inEducationMode(false),
	m_timeEducating(0.0f),
	m_waveNumber(0)
{
	// Read the Invaders' greeting into the string.
	ReadGreeting();
}
GameState::~GameState()
{
}

void GameState::ReadGreeting()
{
	char current;

	m_inputFile.open(m_filePath);
	if ( m_inputFile.is_open() )
	{
		while( m_inputFile.good() )
		{
			current = m_inputFile.get();
			if ( m_inputFile.good() )
			{
				m_explanationString += current;
			}
		}
	}
	// Close our greetings file.
	m_inputFile.close();
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
		m_toMainGame = true;
		return true;
	}

	// 30 seconds have passed.
	float timePassed = Game::GetInstance().GetTotalTime() / 1000.0f;
	if ( timePassed >= SECONDS_TO_CHANGE )
	{
		m_toMainGame = true;
		return true;
	}

	// The player has taken two damage.
	int playerHealthTotal = Game::GetInstance().GetPlayer().GetTotalHealth();
	int playerHealthNow = Game::GetInstance().GetPlayer().GetCurrentHealth();
	if ( playerHealthTotal - playerHealthNow >= DAMAGE_TO_CHANGE )
	{
		m_toMainGame = true;
		return true;
	}

	return false;
}

void GameState::TransitionToMainGameMode(float frameTime)
{
	// Increase the timer.
	m_textTimer += frameTime;

	if ( m_textTimer > CHARACTER_WAIT_TIME )
	{
		unsigned int currentLen = m_currentExplanation.length();
		if ( currentLen < m_explanationString.length() )
		{
			char next = m_explanationString[currentLen];
			// Play a blip sound, unless it's a new line.
			if  ( next != '\n' && next != ' ' )
			{
				Game::GetInstance().GetAudioManager().PlaySoundEffect("tick");
			}

			m_currentExplanation += next;

			m_textTimer = 0.0f;
		}
		else
		{
			if ( m_textTimer > FINAL_WAIT_TIME )
			{
				// Let's do this.
				m_inMainGameMode = true;
				m_toMainGame = false;
				m_paused = false;
			}
		}
	}
}

void GameState::Update(float frameTime)
{
	// Check to see if we need to move to the second game mode.
	if ( !m_inMainGameMode )
	{
		if ( m_toMainGame || ShouldTransitionToMainGameMode() )
		{
			// Do the transition!
			m_paused = true;
			Game::GetInstance().GetInputController().SetControlsBlocked(true);

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

	if ( m_paused && !m_toMainGame )
	{
		RenderPauseScreen();
	}
	else if ( m_paused && m_toMainGame )
	{
		RenderGreeting();
	}

	RenderUI();
}

void GameState::RenderGreeting()
{
	// This is used for rendering the ticking text greeting.
	int semitrans = 100;
	Game::GetInstance().GetRenderer().RenderOverlay(semitrans);

	RECT fontDimensions = Game::GetInstance().GetRenderer().MeasureString(m_currentExplanation);
	int width = fontDimensions.right - fontDimensions.left;
	int height = fontDimensions.bottom - fontDimensions.top;
	float xPos = static_cast<float>( Renderer::GetScreenWidth() - width ) / 2.0f;
	float yPos = static_cast<float>( Renderer::GetScreenHeight() - height ) / 2.0f;

	Game::GetInstance().GetRenderer().DrawText(m_currentExplanation, Vector2(xPos, yPos));
}

void GameState::RenderPauseScreen()
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

void GameState::RenderUI()
{
	// Render the player's score.
	float xPos = static_cast<float>( Renderer::GetScreenWidth() / 2 );
	float yPos = static_cast<float>( Renderer::GetScreenHeight() - 50.0f );
	Vector2 scorePos(xPos, yPos);
	
	std::stringstream ss;
	ss << "Score: " << m_playerScore;
	Game::GetInstance().GetRenderer().DrawText(ss.str(), Vector2(xPos, yPos), true);
}