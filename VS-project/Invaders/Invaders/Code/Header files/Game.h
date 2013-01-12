#ifndef GAME_H
#define GAME_H

//-----------------------------------------------------------------------------
// Game.h 
// The main game class for handling the updating and management of the game.
//-----------------------------------------------------------------------------

// External library include for drawing/updating game.
#include "DiceInvadersLib.h"

#include <string>

#include "InputController.h"
#include "Player.h"
#include "Debug.h"

#ifdef _DEBUG
// Stringstream include, used for converting ints to strings for debugging.
#include <sstream>
#endif

class Game
{
	public:
		static Game& GetInstance()
		{
			static Game instance;
			return instance;
		}
		virtual ~Game();

		// Initialises the game, setting up all necessary data structures.
		void Init();

		// This is the main function for processing the game logic.
		void Run();

		// These three functions handle the three separate logical stages
		// of player interaction with the game: Input, updating, rendering.
		void HandleInput(float frameTime);
		void Update(float frameTime);
		void Render();

		//---------------------------------------------------------------------
		// Accessors

		inline bool IsRunning() { return m_gameRunning; }

		inline Player& GetPlayer() { return *mp_player; }

		inline IDiceInvaders& GetSystem() { return *mp_system; }
		
		inline int GetScreenWidth() { return SCREEN_WIDTH; }

	private:
		// Is the game initialised?
		bool m_initialised;

		// Controls whether the game is still running or not.
		bool m_gameRunning;

		// The elapsed time of the game.
		float m_lastTime;

		// Instance of the external library for world drawing and updating.
		DiceInvadersLib* mp_library;
		char* m_libPath;
		IDiceInvaders* mp_system;

		// Game world size.
		static const int SCREEN_WIDTH = 800;
		static const int SCREEN_HEIGHT = 600;

		// The player object.
		Player* mp_player;

		// TODO: Refactor into a constants class?
		static const int PLAYER_START_X = ( SCREEN_WIDTH / 2 ) - 16;

		// Sprite height plus a bit more for aesthetic padding.
		static const int PLAYER_Y_OFFSET = 40; 
		static const int PLAYER_START_Y = SCREEN_HEIGHT - PLAYER_Y_OFFSET;

		// Private default ctor to facilitate Singleton pattern.
		Game();

		// DO NOT IMPLEMENT
		Game(Game const&);
		void operator=(Game const&);
};

#endif // GAME_H