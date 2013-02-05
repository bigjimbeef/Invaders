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
#include <map>

// Forward declare Game to allow access to Game singleton.
class Game;
class Enemy;

enum eGameState
{
	MENU = 0,
	INITIAL_GAME,
	MAIN_GAME,
	EDUCATION,
	GAME_OVER
};

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
		void RenderLogo();
		void RenderMenu();
		void RenderPauseScreen();
		void RenderGreeting();
		void RenderUI();
		void RenderGameOverScreen();

		// Read the file containing the Invaders' greeting.
		void ReadGreeting();

		// Read the file containing the high score.
		void ReadHighScore();
		// Write the latest high score.
		void WriteHighScore();

		// Increase the wave number, capping at MAX_WAVES
		void IncrementWaveNumber();

		// Used to recalculate game difficulty based on number of enemies alive.
		void RecalculateDifficulty();

		// State transitioning.
		void UpdateState(float frameTime);
		void UpdateTransition(float frameTime);

		// Move to the second game mode, where education is key.
		bool ShouldTransitionToMainGameMode();
		void TransitionToMainGameMode(float frameTime);

		// Lock into educating an invader.
		void StartEducation(Enemy* tutee);
		void EndEducation();
		void UpdateEducation(float frameTime);
		void TransitionToEducation();
		void TransitionFromEducation();

		// Set up the word for teaching with, on our tutee.
		void StartTeaching();

		// Increase the time we've been educating for. This is used when we
		// input the wrong letters for a word.
		void IncTimeEducating(float value);

		//---------------------------------------------------------------------
		// Accessors
		inline void GoToGame() { m_targetState = INITIAL_GAME; }

		inline bool IsPaused() const { return m_paused; }
		inline void SetIsPaused(bool value) { m_paused = value; }

		inline int GetDifficulty() const { return m_difficulty; }
		inline void SetDifficulty(int val) { m_difficulty = val; }

		inline int GetWaveNumber() const { return m_waveNumber; }
		inline void IncrementScore(int amount) { m_playerScore += amount; }
		
		inline bool IsGameOver() const { return m_gameState == GAME_OVER; }
		inline void SetGameOver() { m_gameState = GAME_OVER; }

		inline bool InMenu() const { return m_gameState == MENU; }
		inline bool InMainGameMode() const { return m_gameState == MAIN_GAME; }

		inline Enemy* GetTutee() const { return mp_tutee; }
		
		inline float GetTimeEducating() const { return m_timeEducating; }
		inline static int GetEducationDuration() { return BASE_EDUCATION_TIME; }

		// We consider the point where we are transitioning into education mode
		// as true for this accessor
		inline bool AreEducating() const
			{ return m_gameState == EDUCATION || m_targetState == EDUCATION; }
		inline bool InEducationMode() const { return m_gameState == EDUCATION; }
		inline bool TargettingEducation() const { return m_targetState == EDUCATION; }

	private:
		// Is the game paused?
		bool m_paused;

		// The logo is a map of characters to positions.
		std::map<IDirect3DTexture9*, Vector2> m_logo;
		static const int MAX_LETTERS = 20;		
		float m_letterOffset[MAX_LETTERS];
		float m_wiggleTime[MAX_LETTERS];

		int m_playerScore;
		int m_highScore;
		std::string m_highScoreFilePath;
		std::ofstream m_outputFile;

		int m_difficulty;

		eGameState m_gameState;
		eGameState m_previousState;
		eGameState m_targetState;

		//---------------------------------------------------------------------
		// Transition text.
		// These variables are used when we transition to the main game mode.
		std::string m_greetingFilePath;
		std::ifstream m_inputFile;		
		std::string m_explanationString;
		std::string m_currentExplanation;
		float m_textTimer;

		// Reusable bool to check when we're done transitioning.
		Enemy* mp_tutee;

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

		static const int UI_PADDING = 10;
		static const int UI_PADDING_SMALL = 5;
		static const int TEXT_BLOB_WIDTH = 40;

		static const int LOGO_FONT_SIZE = 100;

		static const int WIGGLE_FREQUENCY_SCALAR = 100;
		static const int WIGGLE_AMPLITUDE = 3;

		// Private default ctor to facilitate Singleton pattern.
		GameState();

		// DO NOT IMPLEMENT
		GameState(GameState const&);
		void operator=(GameState const&);
};

#endif // GAMESTATE_H