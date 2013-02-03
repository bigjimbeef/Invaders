#ifndef GAMESTATE_H
#define GAMESTATE_H

//-----------------------------------------------------------------------------
// GameState.h
// Contains information about the state of the game, like wave number or score.
//-----------------------------------------------------------------------------

#include "PlayerUI.h"

// Forward declare Game to allow access to Game singleton.
class Game;
class Enemy;

class GameState
{
	public:
		static GameState& GetInstance()
		{
			static GameState instance;
			return instance;
		}
		virtual ~GameState();	

		void Update(float frameTime);	

		// Increase the wave number, capping at MAX_WAVES
		void IncrementWaveNumber();

		// Move to the second game mode, where education is key.
		void TransitionToMainGameMode();

		// Lock into educating an invader.
		void StartEducation(Enemy* tutee);
		void EndEducation();
		void TransitionToEducation();
		void TransitionFromEducation();

		//---------------------------------------------------------------------
		// Accessors
		inline int GetWaveNumber() const { return m_waveNumber; }
		inline void IncrementScore(int amount) { m_playerScore += amount; }
		
		inline bool IsGameOver() const { return m_gameOver; }
		inline void SetGameOver() { m_gameOver = true; }

		inline bool InMainGameMode() const { return m_inMainGameMode; }
		inline void SetEducating(bool value) { m_transitioningToEducation = value; }
		
		// We consider the point where we are transitioning into education mode
		// as true for this accessor
		inline bool AreEducating() const 
			{ return m_inEducationMode || m_transitioningToEducation; }

	private:
		int m_playerScore;

		// Is the game finished?
		bool m_gameOver;

		// Reusable bool to check when we're done transitioning.
		bool m_transitioningToEducation;
		bool m_transitioningFromEducation;
		Enemy* mp_tutee;

		// Have we progressed into the second mode?
		bool m_inMainGameMode;
		// Are we currently educating an Invader?
		bool m_inEducationMode;

		// Tracks what enemy wave we are on. This is used as the enemies
		// begin further down the screen in later waves.
		int m_waveNumber;
		static const int MAX_WAVES = 5;

		// Private default ctor to facilitate Singleton pattern.
		GameState();

		// DO NOT IMPLEMENT
		GameState(GameState const&);
		void operator=(GameState const&);
};

#endif // GAMESTATE_H