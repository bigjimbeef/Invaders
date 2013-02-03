#include "GameState.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

GameState::GameState() :
	m_playerScore(0),
	m_gameOver(false),
	m_transitioningToEducation(false),
	m_transitioningFromEducation(false),
	mp_tutee(NULL),
	m_inMainGameMode(true),
	m_inEducationMode(false)
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
}

void GameState::TransitionToEducation()
{
	if ( m_transitioningToEducation )
	{
		float speed = Game::GetInstance().GetSpeedFactor();
		float diff = speed - 0.1f;
		if ( MathsHelper::Abs(diff) < 0.1f ) {
			m_transitioningToEducation = false;
			m_inEducationMode = true;
		}

		MathsHelper::Lerp(speed, 0.1f, 0.025f);
		Game::GetInstance().SetSpeedFactor(speed);
	}
}
void GameState::TransitionFromEducation()
{
	if ( m_transitioningFromEducation )
	{
		float speed = Game::GetInstance().GetSpeedFactor();
		float diff = speed - 0.1f;
		if ( MathsHelper::Abs(diff) < 0.1f ) {
			m_transitioningFromEducation = false;
			m_inEducationMode = false;
		}

		MathsHelper::Lerp(speed, 1.0f, 0.025f);
		Game::GetInstance().SetSpeedFactor(speed);
	}
}

void GameState::StartEducation(Enemy* tutee)
{
	mp_tutee = tutee;
	m_transitioningToEducation = true;
}

void GameState::EndEducation()
{
	mp_tutee = NULL;
	m_transitioningFromEducation = true;
}

void GameState::Update(float frameTime)
{
	if ( m_transitioningToEducation )
	{
		TransitionToEducation();
	}
	else if ( m_transitioningFromEducation )
	{
		TransitionFromEducation();
	}
}