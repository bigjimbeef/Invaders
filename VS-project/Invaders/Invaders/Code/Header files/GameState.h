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
		void RenderUI();

		// This function draws a line above the UI, much like
		// the original game.
		void RenderLine();
		// This function draws the player score, on two lines.
		void RenderScore();
		// The function draws the player's remaining lives.
		void RenderLives();
		// Increase the wave number, capping at MAX_WAVES
		void IncrementWaveNumber();

		//---------------------------------------------------------------------
		// Accessors
		inline int GetWaveNumber() { return m_waveNumber; }
		inline void IncrementScore(int amount) { m_playerScore += amount; }

	private:
		int m_playerScore;

		// Tracks what enemy wave we are on. This is used as the enemies
		// begin further down the screen in later waves.
		int m_waveNumber;
		static const int MAX_WAVES = 5;
		
		// Have we drawn the line at the bottom of the screen?
		bool m_lineDrawn;
		static const int LINE_OFFSET = 60;
		// This was measured through trial and error. 
		static const int LINE_LENGTH = 160;

		static const int SCORE_XPOS = 500;
		static const int UI_OFFSET = 45;
		static const int LINE_HEIGHT = 20;

		static const int LIVES_XPOS = 50;

		// The lives UI is an array of three PlayerUI objects.
		PlayerUI* mp_playerUI[3];

		// Private default ctor to facilitate Singleton pattern.
		GameState();

		// DO NOT IMPLEMENT
		GameState(GameState const&);
		void operator=(GameState const&);
};

#endif // GAMESTATE_H