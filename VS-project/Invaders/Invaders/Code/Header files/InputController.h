#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H

//-----------------------------------------------------------------------------
// InputController.h
// Manages the player input through a single class.
//-----------------------------------------------------------------------------

// Forward declare Game to allow access to Game singleton.
class Game;

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
		void HandleInput(float frameTime);

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