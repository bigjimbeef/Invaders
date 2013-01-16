#ifndef GAMESTATE_H
#define GAMESTATE_H

//-----------------------------------------------------------------------------
// GameState.h
// Contains information about the state of the game, like wave number or score.
//-----------------------------------------------------------------------------

#include "PlayerUI.h"

// Forward declare Game to allow access to Game singleton.
class Game;

class GameState
{
	public:
		static GameState& GetInstance()
		{
			static GameState instance;
			return instance;
		}
		virtual ~GameState();		

		// This function renders the Score and Lives UI.
		void RenderUI(float frameTime);

		// This function draws a line above the UI, much like
		// the original game.
		void RenderLine();
		// This function draws the player score, on two lines.
		void RenderScore();
		// The function draws the player's remaining lives.
		void RenderLives();
		// Increase the wave number, capping at MAX_WAVES
		void IncrementWaveNumber();

		// The game is over, so indicate that in text form.
		void GameOverMessage(float frameTime);

		//---------------------------------------------------------------------
		// Accessors
		inline int GetWaveNumber() const { return m_waveNumber; }
		inline void IncrementScore(int amount) { m_playerScore += amount; }
		
		inline bool IsGameOver() const { return m_gameOver; }
		inline void SetGameOver() { m_gameOver = true; }

	private:
		int m_playerScore;

		// Is the game finished?
		bool m_gameOver;
		const char* m_gameOverMessage;
		int m_gameOverMsgXPos;
		int m_gameOverMsgYPos;
		int m_pressStartXPos;
		int m_pressStartYPos;

		// Tracks what enemy wave we are on. This is used as the enemies
		// begin further down the screen in later waves.
		int m_waveNumber;
		static const int MAX_WAVES = 5;
		
		// Have we drawn the line at the bottom of the screen?
		bool m_lineDrawn;

		//---------------------------------------------------------------------
		// UI positioning constants.
		// Note that these are all just trial and error measurements for
		// aesthetic purposes.
		static const int UI_OFFSET = 35;
		static const int LINE_OFFSET = 45;
		static const int SCORE_OFFSET = 10;

		static const int LINE_LENGTH = 160;

		static const int LIVES_XPOS = 50;
		static const int SCORE_XPOS = 500;
		
		static const int NUM_PADDING_ZEROES = 6;		

		// The lives UI is an array of three PlayerUI objects.
		PlayerUI* mp_playerUI[3];

		// Private default ctor to facilitate Singleton pattern.
		GameState();

		// DO NOT IMPLEMENT
		GameState(GameState const&);
		void operator=(GameState const&);
};

#endif // GAMESTATE_H