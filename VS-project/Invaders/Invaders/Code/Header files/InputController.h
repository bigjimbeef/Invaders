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
		InputController();
		~InputController();

		// DirectX message handler for the game window.
		LRESULT MessageHandler(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);

		// The only function of note, this deals with the player input
		// processing, through use of the DiceInvadersLib functions.
		void HandleInput(float frameTime);

		/*
		// Controls for whilst the game is running.
		void GameControls(const IDiceInvaders::KeyStatus& keystate,
						  float frameTime);

		// Controls for the game over "screen".
		void GameOverScreen(const IDiceInvaders::KeyStatus& keystate);
		*/

	private:
		static const int KEY_ARRAY_SIZE = 256;
		// Which keys are down?
		bool m_isKeyDown[KEY_ARRAY_SIZE];
		bool m_wasKeyDown[KEY_ARRAY_SIZE];
		// How many times have we hit each key?
		int m_keyHit[KEY_ARRAY_SIZE];
		// Is the mouse button pressed?
		int m_mb;
};

#endif // INPUTCONTROLLER_H