#include "GameState.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

GameState::GameState() :
	m_playerScore(0),
	m_gameOver(0),
	m_gameOverMessage("GAME OVER"),
	m_gameOverMsgXPos(0),
	m_gameOverMsgYPos(0),
	m_pressStartXPos(0),
	m_pressStartYPos(0),
	m_waveNumber(0),
	m_lineDrawn(0)
{
	for ( int i = 0; i < Game::GetInstance().GetPlayer().GetTotalHealth(); ++i )
	{
		float xPos = static_cast<float>(LIVES_XPOS + ( i * LIVES_XPOS ));
		float yPos = 
			static_cast<float>(Game::GetScreenHeight() - UI_OFFSET);

		// Create and initialise the player UI elements.
		mp_playerUI[i] = new PlayerUI(xPos, yPos);
	}

	// Initialise the position for the Game Over UI.
	// Please note that I am unable to measure the string, and as such
	// I am simply hard-coding approximate string lengths.
	int messageXLength = 80;
	int messageYLength = 15;
	m_gameOverMsgXPos = ( Game::GetScreenWidth() - messageXLength ) / 2;
	m_gameOverMsgYPos = ( Game::GetScreenHeight() - messageYLength ) / 2;

	int pressStartXLength = 160;
	m_pressStartXPos = ( Game::GetScreenWidth() - pressStartXLength ) / 2;
	// Offset by height of string.
	m_pressStartYPos = m_gameOverMsgYPos + messageYLength; 
}
GameState::~GameState()
{
	for ( int i = 0; i < Game::GetInstance().GetPlayer().GetTotalHealth(); ++i )
	{
		delete mp_playerUI[i];
		mp_playerUI[i] = NULL;
	}
}

void GameState::RenderLine()
{
	// Draw the line which contains the player health/score UI.
	int yPos = Game::GetScreenHeight() - LINE_OFFSET;
	std::string sLine = "";
	for( int i = 0; i < LINE_LENGTH; ++i )
	{
		sLine += "-";
	}
	Game::GetInstance().GetSystem().drawText(0, yPos, sLine.c_str());
}

void GameState::RenderScore()
{
	int scorePos = 
		Game::GetScreenHeight() - UI_OFFSET + SCORE_OFFSET;

	// Convert the score to a std::string with a stringstream.
	std::stringstream ss;
	ss << m_playerScore;
	std::string score = ss.str();

	for ( unsigned int i = 0; i <= NUM_PADDING_ZEROES; ++i )
	{
		if ( score.length() < i )
		{
			score = "0" + score;
		}
	}
	score = "Score:    " + score;

	// Render the player score, padded with zeroes.
	Game::GetInstance().GetSystem().drawText(SCORE_XPOS,
											 scorePos, 
											 score.c_str());
}

void GameState::RenderLives()
{
	int currentHealth = Game::GetInstance().GetPlayer().GetCurrentHealth();
	
	// Draw a UI element for each unit of health.
	for ( int i = 0; i < currentHealth; ++i )
	{
		mp_playerUI[i]->Render();
	}
}

void GameState::RenderUI(float frameTime)
{
	// Draw the line at the top of the UI.
	RenderLine();

	// Draw the score, in two parts.
	RenderScore();

	// Draw the player lives UI.
	RenderLives();

	// If the game is finished ...
	if ( m_gameOver )
	{
		// ... render the game over message.
		GameOverMessage(frameTime);
	}
}

void GameState::IncrementWaveNumber()
{ 
	m_waveNumber++;

	if ( m_waveNumber > MAX_WAVES ) 
	{ 
		m_waveNumber = 0; 
	} 
}

void GameState::GameOverMessage(float frameTime)
{
	// Draw the game over messages.
	Game::GetInstance().GetSystem().drawText(
	    m_gameOverMsgXPos, m_gameOverMsgYPos, m_gameOverMessage);
	Game::GetInstance().GetSystem().drawText(
	    m_pressStartXPos, m_pressStartYPos, "Please press Fire to exit");
}
