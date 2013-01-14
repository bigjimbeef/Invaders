#ifndef GAME_H
#define GAME_H

//-----------------------------------------------------------------------------
// Game.h 
// The main game class for handling the updating and management of the game.
//-----------------------------------------------------------------------------

// External library include for drawing/updating game.
#include "DiceInvadersLib.h"

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

//#define MEMLEAK_DEBUGGING
#ifdef MEMLEAK_DEBUGGING
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>

	#ifdef _DEBUG
	   #ifndef DBG_NEW
		  #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
		  #define new DBG_NEW
	   #endif
	#endif  // _DEBUG
#endif

#ifdef _DEBUG
#include <vld.h>
#endif

#ifdef _DEBUG
#include "Debug.h"
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
		inline int GetScreenHeight() { return SCREEN_HEIGHT; }

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

		// Game world dimensions
		static const int SCREEN_WIDTH = 640;
		static const int SCREEN_HEIGHT = 480;

		// The player object.
		Player* mp_player;

		// TODO: Refactor into a constants class?
		static const int PLAYER_START_X = ( SCREEN_WIDTH / 2 ) - 16;

		// Sprite height plus a bit more for aesthetic padding.
		static const int PLAYER_Y_OFFSET = 100; 
		static const int PLAYER_START_Y = SCREEN_HEIGHT - PLAYER_Y_OFFSET;

		// Private default ctor to facilitate Singleton pattern.
		Game();

		// DO NOT IMPLEMENT
		Game(Game const&);
		void operator=(Game const&);
};

#endif // GAME_H