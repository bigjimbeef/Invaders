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

	// Get the player X position for calculating if we can move.
	float playerX = Game::GetInstance().GetPlayer().GetPosition().x;
	float rightBounds =
		static_cast<float>(
			Game::GetInstance().GetScreenWidth() - 
			Game::GetInstance().GetSpriteSize()
		);

	// Process the key state.
	if ( keystate.left )
	{
		if ( playerX > 0.0f )
		{
			// Move the player in negative X.
			Game::GetInstance().GetPlayer().Move(-1, frameTime);
		}
	}
	
	if ( keystate.right )
	{
		if ( playerX < rightBounds )
		{
			// Move the player in position X.
			Game::GetInstance().GetPlayer().Move(1, frameTime);
		}
	}
	
	if ( keystate.fire )
	{
		// We can only shoot again if we've let go of the key.
		// Don't really want the player to be able to machine gun fire
		// by just holding down the spacebar!
		if ( !m_wasFiring)
		{
			// Make the player fire a rocket.
			Game::GetInstance().GetPlayer().Fire();
		}
	}

	// Cache whether or not we were firing.
	m_wasFiring = keystate.fire;

#ifdef _DEBUG
	Position playerPos = Game::GetInstance().GetPlayer().GetPosition();
	
	std::stringstream ss;
	ss << "Player is at: [" << playerPos.x << ", " << playerPos.y << "]";
	std::string pos = ss.str();

	Game::GetInstance().GetSystem().drawText(0, 0, pos.c_str());
#endif
}
