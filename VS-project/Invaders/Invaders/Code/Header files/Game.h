#ifndef GAME_H
#define GAME_H

//-----------------------------------------------------------------------------
// Game.h 
// The main game class for handling the updating and management of the game.
//-----------------------------------------------------------------------------

// External library include for drawing/updating game.
#include "DiceInvadersLib.h"

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
		void HandleInput();
		void Update();
		void Render();

		//---------------------------------------------------------------------
		// Accessors

		inline bool IsRunning() { return m_gameRunning; }

		// inline Player& GetPlayer() { return *mp_player; }
		

	private:
		// Is the game initialised?
		bool m_initialised;

		// Controls whether the game is still running or not.
		bool m_gameRunning;

		// Instance of the external library for world drawing and updating.
		DiceInvadersLib* mp_library;
		char* m_libPath;
		IDiceInvaders* mp_system;

		// Private default ctor to facilitate Singleton pattern.
		Game();

		// DO NOT IMPLEMENT
		Game(Game const&);
		void operator=(Game const&);
};

#endif // GAME_H