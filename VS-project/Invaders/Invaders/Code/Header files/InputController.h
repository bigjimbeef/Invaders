#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H

//-----------------------------------------------------------------------------
// InputController.h
// Manages the player input through a single class.
//-----------------------------------------------------------------------------

#include "Renderer.h"

// Forward declare Game to allow access to Game singleton.
class Game;

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

		// Check if a specific key is down.
		bool IsKeyDown(unsigned int key);
		// Check if a specific key was down last frame.
		bool WasKeyDown(unsigned int key);

		//---------------------------------------------------------------------
		// Accessors

		inline void SetControlsBlocked(bool blocked) 
			{ m_controlsBlocked = blocked; }

	private:
		// Controls for whilst the game is running.
		void GameControls(float frameTime);

		// Controls that are always in use unless specifically blocked.
		void MenuControls();

		// Send the characters pressed to the Word objects in the game.
		void SendCharacters();

		// Determine which is the first (alphabetically) key currently held.
		char GetHeldKey();

		// How long was this frame.
		float m_frameTime;

		static const int KEY_ARRAY_SIZE = 256;

		// Which keys are down?
		bool m_isKeyDown[KEY_ARRAY_SIZE];
		bool m_wasKeyDown[KEY_ARRAY_SIZE];
		// Is the mouse button pressed?
		int m_mb;

		// Are the menu controls blocked? Only happens during transition
		// to main game mode.
		bool m_controlsBlocked;

		// Constants for accessing keys array.
		static const int LEFT_ARROW = 37;
		static const int UP_ARROW = 38;
		static const int RIGHT_ARROW = 39;
		static const int DOWN_ARROW = 40;
		static const int SPACEBAR = 32;
		static const int RETURN = 13;

		static const int A_KEY = 65;
};

#endif // INPUTCONTROLLER_H