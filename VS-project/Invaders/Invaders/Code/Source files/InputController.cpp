#include "InputController.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

InputController::InputController()
{
}
InputController::~InputController()
{
}

void InputController::HandleInput(float frameTime)
{
	// Populate the key state from the game system.
	IDiceInvaders::KeyStatus keystate;
	Game::GetInstance().GetSystem().getKeyStatus(keystate);

	// Process the key state.
	if ( keystate.left )
	{
		// Move the player in negative X.
		Game::GetInstance().GetPlayer().Move(-1, frameTime);
	}
	else if ( keystate.right )
	{
		// Move the player in position X.
		Game::GetInstance().GetPlayer().Move(1, frameTime);
	}
	else if ( keystate.fire )
	{
		// We can only shoot again if we've let go of the key.
		// Don't really want the player to be able to machine gun fire
		// by just holding down the spacebar!
		if ( !m_wasFiring)
		{
			// Game::GetInstance().GetPlayer()->Fire();
		}

		// Cache whether or not we were firing.
		m_wasFiring = keystate.fire;
	}

#ifdef _DEBUG
	Position playerPos = Game::GetInstance().GetPlayer().GetPosition();
	
	std::stringstream ss;
	ss << "Player is at: [" << playerPos.x << ", " << playerPos.y << "]";
	std::string pos = ss.str();

	Game::GetInstance().GetSystem().drawText(0, 0, pos.c_str());
#endif
}
