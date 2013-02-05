#include "GameState.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

GameState::GameState() :
	m_paused(false),
	m_playerScore(0),
	m_highScore(0),
	m_difficulty(0),
	m_gameState(MAIN_GAME),
	m_previousState(MAIN_GAME),
	m_targetState(MAIN_GAME),
	m_highScoreFilePath("Code/Resource files/hiscore.txt"),
	m_greetingFilePath("Code/Resource files/greeting.txt"),
	m_explanationString(""),
	m_currentExplanation(""),
	m_textTimer(0.0f),
	mp_tutee(NULL),
	m_timeEducating(0.0f),
	m_waveNumber(0)
{
	// Read the Invaders' greeting into the string.
	ReadGreeting();

	// Read the high score into memory.
	ReadHighScore();

	m_logo = std::map<IDirect3DTexture9*, Vector2>();
	// Initialise the logo.
	std::string title = "invaders";
	// Work out how much space this will occupy.
	int width = title.length() * LOGO_FONT_SIZE;
	float baseXPos = 
		static_cast<float>( Renderer::GetScreenWidth() - width ) / 2.0f;
	float baseYPos = 
		static_cast<float>( Renderer::GetScreenHeight() - LOGO_FONT_SIZE ) / 2.0f;
	Vector2 basePos(baseXPos, baseYPos);
	for ( unsigned int i = 0; i < title.length(); ++i )
	{
		char current = title[i];

		Vector2 thisPos = basePos;
		thisPos.x += ( i * LOGO_FONT_SIZE );

		IDirect3DTexture9* sprite = 
			Game::GetInstance().GetResourceManager().GetLetterSprite(current);

		m_logo.insert(
			std::pair<IDirect3DTexture9*, Vector2>(sprite, thisPos)
		);
	}

	for ( unsigned int i = 0; i < title.length(); ++i )
	{
		m_letterOffset[i] = 0.0f;
		m_wiggleTime[i] = static_cast<float>(i);
	}
}
GameState::~GameState()
{
}

void GameState::ReadGreeting()
{
	char current;

	m_inputFile.open(m_greetingFilePath);
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

void GameState::ReadHighScore()
{
	int highest = 0;

	m_inputFile.open(m_highScoreFilePath);
	if ( m_inputFile.is_open() )
	{
		std::string line;
		while( std::getline(m_inputFile, line) )
		{
			int number;
			std::istringstream ( line ) >> number;

			if ( number > highest )
			{
				highest = number;
			}
		}
	}

	// Cache the high score.
	m_highScore = highest;

	// Close our greetings file.
	m_inputFile.close();
}
void GameState::WriteHighScore()
{
	m_outputFile.open(m_highScoreFilePath);
	int targetScore = ( m_highScore > m_playerScore ) ? m_highScore : m_playerScore;
	m_outputFile << targetScore;
	m_outputFile.close();
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
	float target = 0.1f;
	float speed = Game::GetInstance().GetSpeedFactor();
	float diff = speed - target;

	MathsHelper::Lerp(speed, target, 0.025f);
	Game::GetInstance().SetSpeedFactor(speed);

	if ( MathsHelper::Abs(diff) < 0.1f ) {
		m_gameState = EDUCATION;

		// Set-up the word to educate with.
		StartTeaching();
	}
}
void GameState::TransitionFromEducation()
{
	float target = 1.0f;
	float speed = Game::GetInstance().GetSpeedFactor();
	float diff = speed - target;

	MathsHelper::Lerp(speed, target, 0.025f);
	Game::GetInstance().SetSpeedFactor(speed);

	if ( MathsHelper::Abs(diff) < 0.1f ) {
		// Lock back to 100% play speed.
		Game::GetInstance().SetSpeedFactor(target);

		m_gameState = MAIN_GAME;
	}
}

void GameState::StartEducation(Enemy* tutee)
{
	mp_tutee = tutee;

	m_targetState = EDUCATION;
}

void GameState::EndEducation()
{
	m_timeEducating = 0.0f;
	mp_tutee = NULL;

	m_targetState = MAIN_GAME;
}

bool GameState::ShouldTransitionToMainGameMode()
{
	// We transition when we have either:
	// 10 kills.
	int remaining = Game::GetInstance().GetEnemyManager().GetRemainingEnemies();
	int total = Game::GetInstance().GetEnemyManager().GetTotalEnemies();
	if ( total - remaining >= KILLS_TO_CHANGE )
	{
		m_targetState = MAIN_GAME;
		return true;
	}

	// 30 seconds have passed.
	float timePassed = Game::GetInstance().GetTotalTime() / 1000.0f;
	if ( timePassed >= SECONDS_TO_CHANGE )
	{
		m_targetState = MAIN_GAME;
		return true;
	}

	// The player has taken two damage.
	int playerHealthTotal = Game::GetInstance().GetPlayer().GetTotalHealth();
	int playerHealthNow = Game::GetInstance().GetPlayer().GetCurrentHealth();
	if ( playerHealthTotal - playerHealthNow >= DAMAGE_TO_CHANGE )
	{
		m_targetState = MAIN_GAME;
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
				m_gameState = MAIN_GAME;

				m_paused = false;
			}
		}
	}
}

void GameState::UpdateEducation(float frameTime)
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

void GameState::UpdateState(float frameTime)
{
	switch(m_gameState)
	{
	case MENU:
		m_paused = true;

		for ( unsigned int i = 0; i < m_logo.size(); ++i )
		{
			m_wiggleTime[i] += 
				static_cast<float>(frameTime / WIGGLE_FREQUENCY_SCALAR);
			float deviation = 
				static_cast<float>(WIGGLE_AMPLITUDE * sin(m_wiggleTime[i]));
			m_letterOffset[i] = deviation;
		}

		break;
	case INITIAL_GAME:
		// Check if it's time to move to the main game.
		if ( ShouldTransitionToMainGameMode() )
		{
			// Do the transition!
			m_paused = true;
			Game::GetInstance().GetInputController().SetControlsBlocked(true);

			// We are moving to the main game.
			m_targetState = MAIN_GAME;
		}
		break;
	case MAIN_GAME:
		// Nothing special to be done here.
		break;
	case EDUCATION:
		if ( m_targetState != MAIN_GAME )
		{
			// Update the progress of the Invader's education.
			UpdateEducation(frameTime);
		}
		break;
	case GAME_OVER:
		// Don't need to be updating game state anymore.
		break;
	default:
		break;
	}

	// Cache the previous state.
	m_previousState = m_gameState;
}

void GameState::UpdateTransition(float frameTime)
{
	switch(m_targetState)
	{
	case MENU:
		break;
	case INITIAL_GAME:
		// Can only transition here from the menu.
		// Start the music!
		Game::GetInstance().GetAudioManager().PlayMusic();
		m_paused = false;
		m_gameState = INITIAL_GAME;
		break;
	case MAIN_GAME:
		if ( m_previousState == INITIAL_GAME )
		{
			// To the main game mode.. stat!
			TransitionToMainGameMode(frameTime);
		}
		else if ( m_previousState == EDUCATION )
		{
			TransitionFromEducation();
		}

		break;
	case EDUCATION:
		if ( m_previousState == MAIN_GAME )
		{
			TransitionToEducation();
		}
		
		break;
	case GAME_OVER:
		// Doesn't matter how we ended up here.
		break;
	default:
		break;
	}
}

void GameState::Update(float frameTime)
{
	// Update our current state.
	UpdateState(frameTime);

	// Update the transitioning, if we are doing so.
	UpdateTransition(frameTime);
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
	Vector2 pos(0.0f, 100.0f);
	std::stringstream ss;
	int score = GameState::GetInstance().GetDifficulty();
	ss << "Difficulty: " << score;
	Game::GetInstance().GetRenderer().DrawText(ss.str(), pos);

	ss.str("");
	ss << "Prev: " << m_previousState;
	pos.y += 30.0f;
	Game::GetInstance().GetRenderer().DrawText(ss.str(), pos);

	ss.str("");
	ss << "Curr: " << m_gameState;
	pos.y += 30.0f;
	Game::GetInstance().GetRenderer().DrawText(ss.str(), pos);

	ss.str("");
	ss << "Next: " << m_targetState;
	pos.y += 30.0f;
	Game::GetInstance().GetRenderer().DrawText(ss.str(), pos);
#endif

	switch(m_gameState)
	{
	case MENU:
		RenderMenu();
		break;
	case INITIAL_GAME:
		if ( m_paused && m_targetState == MAIN_GAME )
		{
			RenderGreeting();
		}
		else if ( m_paused )
		{
			RenderPauseScreen();
		}
		break;
	case MAIN_GAME:
		if ( m_paused )
		{
			RenderPauseScreen();
		}
		break;
	case EDUCATION:
		break;
	case GAME_OVER:
		// Not done here. It must be drawn above all else.
		break;
	default:
		break;
	}

	if ( m_gameState != MENU )
	{
		RenderUI();
	}

	if ( m_gameState == GAME_OVER )
	{
		RenderGameOverScreen();
	}
}

void GameState::RenderLogo()
{
	std::map<IDirect3DTexture9*, Vector2>::const_iterator it = m_logo.begin();
	int count = 0;
	for ( it; it != m_logo.end(); ++it )
	{
		Vector2 pos = (*it).second;
		float yPos = pos.y + m_letterOffset[count];

		Game::GetInstance().GetRenderer().DrawSprite(
			it->first, pos.x, yPos, LOGO_FONT_SIZE, LOGO_FONT_SIZE
		);

		count++;
	}
}

void GameState::RenderMenu()
{
	// This is used for rendering the ticking text greeting.
	int opaque = 255;
	Game::GetInstance().GetRenderer().RenderOverlay(opaque);

	// Render the floating logo.
	RenderLogo();

	std::string menu = "Press spacebar to begin!";
	RECT fontDimensions = Game::GetInstance().GetRenderer().MeasureString(menu, true);
	int width = fontDimensions.right - fontDimensions.left;
	int height = fontDimensions.bottom - fontDimensions.top;
	float xPos = static_cast<float>( Renderer::GetScreenWidth() - width ) / 2.0f;
	float yPos = static_cast<float>( Renderer::GetScreenHeight() - height ) / 2.0f;
	yPos += 100.0f;

	Game::GetInstance().GetRenderer().DrawText(menu, Vector2(xPos, yPos), true);
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
	std::stringstream ss;
	ss << m_playerScore;

	// Fit the player's score to the screen right.
	RECT fontDimensions = 
		Game::GetInstance().GetRenderer().MeasureString(ss.str(), true);
	int width = fontDimensions.right - fontDimensions.left;
	int height = fontDimensions.bottom - fontDimensions.top;
	float xPos = 
		static_cast<float>(Renderer::GetScreenWidth() - width - UI_PADDING);
	float yPos = 
		static_cast<float>(Renderer::GetScreenHeight() - height);

	// Draw the background.
	Game::GetInstance().GetRenderer().DrawFilledRect(
		0.0f, yPos, 
		Renderer::GetScreenWidth(), height,
		Renderer::GetColour(0, 0, 100) // Dark blue
	);
	Game::GetInstance().GetRenderer().DrawFilledRect(
		0.0f, 0.0f, 
		Renderer::GetScreenWidth(), height,
		Renderer::GetColour(0, 0, 100) // Dark blue
	);

	// Render the player's current score.
	Game::GetInstance().GetRenderer().DrawText(ss.str(), Vector2(xPos, yPos), true);

	// Render the player's remaining life.
	int remaining = Game::GetInstance().GetPlayer().GetCurrentHealth();
	// Bright green if n == 3, Orange if n == 2, Red if n == 1
	DWORD colour = Renderer::GetColour(0, 255, 0);
	if ( remaining == 2)
	{
		colour = Renderer::GetColour(255, 127, 0);
	}
	else if ( remaining == 1 )
	{
		colour = Renderer::GetColour(255, 0, 0);
	}
	
	int blobHeight = height - ( UI_PADDING_SMALL * 2 );
	
	for ( int i = 0; i < remaining; ++i )
	{
		float blobXPos = 
			static_cast<float>(UI_PADDING + ( i * (UI_PADDING + TEXT_BLOB_WIDTH) ));
		float blobYPos = 
			static_cast<float>(yPos + UI_PADDING_SMALL);

		Game::GetInstance().GetRenderer().DrawFilledRect(
			blobXPos, blobYPos, 
			TEXT_BLOB_WIDTH, blobHeight,
			colour
		);
	}

	// Render the high score.
	ss.str("");
	int targetScore = ( m_highScore > m_playerScore ) ? m_highScore : m_playerScore;
	ss << "HI-SCORE:     " << targetScore;

	// Fit the player's score to the screen right.
	fontDimensions = 
		Game::GetInstance().GetRenderer().MeasureString(ss.str(), true);
	width = fontDimensions.right - fontDimensions.left;
	height = fontDimensions.bottom - fontDimensions.top;
	xPos = static_cast<float>( Renderer::GetScreenWidth() - width ) / 2.0f;
	yPos = 0.0f;
	// Now render the high score.
	Game::GetInstance().GetRenderer().DrawText(ss.str(), Vector2(xPos, yPos), true);	
}

void GameState::RenderGameOverScreen()
{
	int semitrans = 220;
	Game::GetInstance().GetRenderer().RenderOverlay(semitrans);

	std::string paused = "GAME OVER!\n\nPress fire to exit.";
	RECT fontDimensions = Game::GetInstance().GetRenderer().MeasureString(paused, true);
	int width = fontDimensions.right - fontDimensions.left;
	int height = fontDimensions.bottom - fontDimensions.top;

	float xPos = static_cast<float>( Renderer::GetScreenWidth() - width ) / 2.0f;
	float yPos = static_cast<float>( Renderer::GetScreenHeight() - height ) / 2.0f;

	Game::GetInstance().GetRenderer().DrawText(paused, Vector2(xPos, yPos), true);
}