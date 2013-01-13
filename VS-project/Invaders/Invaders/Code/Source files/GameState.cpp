#include "GameState.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

GameState::GameState() :
	m_playerScore(0),
	m_waveNumber(0),
	m_lineDrawn(0)
{
	for ( int i = 0; i < Game::GetInstance().GetPlayer().GetTotalHealth(); ++i )
	{
		float xPos = static_cast<float>(LIVES_XPOS + ( i * LIVES_XPOS ));
		float yPos = static_cast<float>(Game::GetInstance().GetScreenHeight() - UI_OFFSET);

		// Create and initialise the player UI elements.
		mp_playerUI[i] = new PlayerUI(xPos, yPos);
		mp_playerUI[i]->Init();
	}
}
GameState::~GameState()
{
	for ( int i = 0; i < Game::GetInstance().GetPlayer().GetTotalHealth(); ++i )
	{
		delete[] mp_playerUI[i];
	}
}

void GameState::RenderLine()
{
	// Draw the line which contains the player health/score UI.
	int yPos = Game::GetInstance().GetScreenHeight() - LINE_OFFSET;
	std::string sLine = "";
	for( int i = 0; i < LINE_LENGTH; ++i )
	{
		sLine += "-";
	}
	Game::GetInstance().GetSystem().drawText(0, yPos, sLine.c_str());
}

void GameState::RenderScore()
{
	int lineOneYPos = Game::GetInstance().GetScreenHeight() - UI_OFFSET;
	int lineTwoYPos = ( Game::GetInstance().GetScreenHeight() - UI_OFFSET ) + LINE_HEIGHT;

	// Convert the score to a std::string with a stringstream.
	std::stringstream ss;
	ss << m_playerScore;
	std::string score = ss.str();

	// Render the title "Score:"
	Game::GetInstance().GetSystem().drawText(SCORE_XPOS, lineOneYPos, "Score:");
	// Render the actual player score.
	Game::GetInstance().GetSystem().drawText(SCORE_XPOS, lineTwoYPos, score.c_str());
}

void GameState::RenderLives()
{
	for ( int i = 0; i < Game::GetInstance().GetPlayer().GetCurrentHealth(); ++i )
	{
		mp_playerUI[i]->Render();
	}
}

void GameState::RenderUI()
{
	// Draw the line at the top of the UI.
	RenderLine();

	// Draw the score, in two parts.
	RenderScore();

	// Draw the player lives UI.
	RenderLives();
}