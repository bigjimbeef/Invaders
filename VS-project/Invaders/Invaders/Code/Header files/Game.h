#ifndef GAME_H
#define GAME_H

//-----------------------------------------------------------------------------
// Game.h 
// The main game class for handling the updating and management of the game.
//-----------------------------------------------------------------------------

// External library include for drawing/updating game.
#include "DiceInvadersLib.h"

// FMOD header include, for audio engine initialisation.
#include "fmod.h"

#include <string>
// Stringstream include, used for converting ints to strings.
#include <sstream>

#include "InputController.h"
#include "Player.h"
#include "InputController.h"
#include "ProjectileManager.h"
#include "GameState.h"
#include "EnemyManager.h"
#include "ResourceManager.h"
#include "MathsHelper.h"
#include "AudioManager.h"
#include "Renderer.h"

class Game
{
	public:
		static Game& GetInstance()
		{
			static Game instance;
			return instance;
		}
		~Game();

		// This is the main function for processing the game logic.
		void Run();

		// These three functions handle the three separate logical stages
		// of player interaction with the game: Input, updating, rendering.
		void HandleInput(float frameTime);
		void Update(float frameTime);
		void Render();

		//---------------------------------------------------------------------
		// Accessors

		inline bool IsRunning() const { return m_gameRunning; }
		inline void SetRunning(bool running) { m_gameRunning = running; }

		inline Renderer& GetRenderer() const { return *mp_renderer; }
		inline Player& GetPlayer() const { return *mp_player; }
		inline InputController& GetInputController() const { 
			return *mp_inputController; }

		inline IDiceInvaders& GetSystem() const { return *mp_system; }

		inline float GetSpeedFactor() const { return m_gameSpeedFactor; }
		void SetSpeedFactor(float value);
		
		static int GetScreenWidth() { return SCREEN_WIDTH; }
		static int GetScreenHeight() { return SCREEN_HEIGHT; }
		static int GetSpriteSize() { return SPRITE_SIZE; }

	private:

		// Gets the current game time, in ms.
		float GetGameTime();

		// Is the game initialised?
		bool m_initialised;

		// Controls whether the game is still running or not.
		bool m_gameRunning;

		// The frequency the game is running at.
		LARGE_INTEGER m_frequency;
		// The time that the game was started.
		float m_startTime;
		// The total elapsed time of the game.
		float m_totalTime;
		// The time elapsed this frame.
		float m_lastFrameTime;

		// Do we want to run to a capped fps? If so, how much?
		bool m_frameRateCapped;
		int m_frameRateCap;

		// Instance of the external library for world drawing and updating.
		DiceInvadersLib* mp_library;
		const char* mp_libPath;
		IDiceInvaders* mp_system;
		

		// Game world dimensions
		static const int SCREEN_WIDTH = 640;
		static const int SCREEN_HEIGHT = 480;
		static const int SPRITE_SIZE = 32;

		// The game renderer.
		Renderer* mp_renderer;

		// The player object.
		Player* mp_player;

		// The Audio Manager
		AudioManager* mp_audioManager;

		// Handles player input via DirectX messages.
		InputController* mp_inputController;

		// How fast is the game currently going?
		float m_gameSpeedFactor;

		static const int PLAYER_START_X = ( SCREEN_WIDTH / 2 ) - 16;

		// Sprite height plus a bit more for aesthetic padding.
		static const int PLAYER_Y_OFFSET = 70; 
		static const int PLAYER_START_Y = SCREEN_HEIGHT - PLAYER_Y_OFFSET;

		// Private default ctor to facilitate Singleton pattern.
		Game();

		// DO NOT IMPLEMENT
		Game(Game const&);
		void operator=(Game const&);
};

#endif // GAME_H