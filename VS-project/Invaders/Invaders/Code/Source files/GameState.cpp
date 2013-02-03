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
			m_inEducationMode = false;
		}
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

void GameState::StartTeaching()
{
	// Ensure we don't try to teach the aether.
	if ( mp_tutee != NULL )
	{
		mp_tutee->GenerateWord();
	}
}