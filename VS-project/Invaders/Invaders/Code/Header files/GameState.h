#ifndef GAMESTATE_H
#define GAMESTATE_H

//-----------------------------------------------------------------------------
// GameState.h
// Contains information about the state of the game, like wave number or score.
//-----------------------------------------------------------------------------

#include "PlayerUI.h"

#include <string>
#include <istream>
#include <fstream>

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
		// The render function is used entirely to render UI elements and overlays.
		void Render();
		void RenderPauseScreen();
		void RenderGreeting();
		void RenderUI();

		// Read the file containing the Invaders' greeting.
		void ReadGreeting();

		// Increase the wave number, capping at MAX_WAVES
		void IncrementWaveNumber();

		// Used to recalculate game difficulty based on number of enemies alive.
		void RecalculateDifficulty();

		// Move to the second game mode, where education is key.
		bool ShouldTransitionToMainGameMode();
		void TransitionToMainGameMode(float frameTime);

		// Lock into educating an invader.
		void StartEducation(Enemy* tutee);
		void EndEducation();
		void TransitionToEducation();
		void TransitionFromEducation();

		// Set up the word for teaching with, on our tutee.
		void StartTeaching();

		// Increase the time we've been educating for. This is used when we
		// input the wrong letters for a word.
		void IncTimeEducating(float value);

		//---------------------------------------------------------------------
		// Accessors
		inline bool IsPaused() const { return m_paused; }
		inline void SetIsPaused(bool value) { m_paused = value; }

		inline int GetDifficulty() const { return m_difficulty; }
		inline void SetDifficulty(int val) { m_difficulty = val; }

		inline int GetWaveNumber() const { return m_waveNumber; }
		inline void IncrementScore(int amount) { m_playerScore += amount; }
		
		inline bool IsGameOver() const { return m_gameOver; }
		inline void SetGameOver() { m_gameOver = true; }

		inline bool InMainGameMode() const { return m_inMainGameMode; }
		inline void SetEducating(bool value) { m_transitioningToEducation = value; }

		inline Enemy* GetTutee() const { return mp_tutee; }
		
		inline float GetTimeEducating() const { return m_timeEducating; }
		inline static int GetEducationDuration() { return BASE_EDUCATION_TIME; }

		// We consider the point where we are transitioning into education mode
		// as true for this accessor
		inline bool AreEducating() const 
			{ return m_inEducationMode || m_transitioningToEducation; }
		inline bool InEducationMode() const { return m_inEducationMode; }

	private:
		// Is the game paused?
		bool m_paused;

		int m_playerScore;

		int m_difficulty;

		// Is the game finished?
		bool m_gameOver;

		// Are we on our way to the main game.
		bool m_toMainGame;

		//---------------------------------------------------------------------
		// Transition text.
		// These variables are used when we transition to the main game mode.
		std::string m_filePath;
		std::ifstream m_inputFile;		
		std::string m_explanationString;
		std::string m_currentExplanation;
		float m_textTimer;


		// Reusable bool to check when we're done transitioning.
		bool m_transitioningToEducation;
		bool m_transitioningFromEducation;
		Enemy* mp_tutee;

		// Have we progressed into the second mode?
		bool m_inMainGameMode;
		// Are we currently educating an Invader?
		bool m_inEducationMode;

		// How long have we been educating for?
		float m_timeEducating;

		// Tracks what enemy wave we are on. This is used as the enemies
		// begin further down the screen in later waves.
		int m_waveNumber;

		static const int KILLS_TO_CHANGE = 10;
		static const int SECONDS_TO_CHANGE = 30;
		static const int DAMAGE_TO_CHANGE = 2;

		static const int MAX_WAVES = 5;
		static const int BASE_EDUCATION_TIME = 3;
		static const int DIFFICULTY_PER_WAVE = 20;
		static const int MAX_DIFFICULTY_PER_WAVE = 50;

		// Transition variables are in ms for ease.
		static const int CHARACTER_WAIT_TIME = 50;
		static const int FINAL_WAIT_TIME = 3000;

		// Private default ctor to facilitate Singleton pattern.
		GameState();

		// DO NOT IMPLEMENT
		GameState(GameState const&);
		void operator=(GameState const&);
};

#endif // GAMESTATE_H