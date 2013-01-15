#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H

//-----------------------------------------------------------------------------
// InputController.h
// Manages the player input through a single class.
//-----------------------------------------------------------------------------

// Forward declare Game to allow access to Game singleton.
class Game;

// Need access to the IDiceInvaders::KeyStatus enum.
#include "DiceInvadersLib.h"

class InputController
{
	public:
		static InputController& GetInstance()
		{
			static InputController instance;
			return instance;
		}
		virtual ~InputController();

		// The only function of note, this deals with the player input
		// processing, through use of the DiceInvadersLib functions.
		void HandleInput(const float frameTime);

		// Controls for whilst the game is running.
		void GameControls(const IDiceInvaders::KeyStatus keystate,
						  const float frameTime);

		// Controls for the game over "screen".
		void GameOverControls(const IDiceInvaders::KeyStatus keystate);

	private:
		// Holds the state of the player keys at any given time.
		bool m_wasFiring;

		// Private default ctor to facilitate Singleton pattern.
		InputController();

		// DO NOT IMPLEMENT
		InputController(InputController const&);
		void operator=(InputController const&);
};

#endif // INPUTCONTROLLER_H