#include "InputController.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

InputController::InputController() :
	m_mb(0)
{
	for( int i = 0; i < KEY_ARRAY_SIZE; ++i )
	{
		m_isKeyDown[i] = false;
		m_wasKeyDown[i] = false;
		m_keyHit[i] = 0;
	}
}
InputController::~InputController()
{
}

LRESULT InputController::MessageHandler(HWND hWnd, UINT msg, 
	WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_LBUTTONDOWN:
		SetCapture(hWnd);
		m_mb|=1;
		m_isKeyDown[VK_LBUTTON]=true;
		m_keyHit[VK_LBUTTON]++;
		break;
	case WM_RBUTTONDOWN:
		SetCapture(hWnd);
		m_isKeyDown[VK_RBUTTON]=true;
		m_keyHit[VK_RBUTTON]++;
		m_mb|=2;
		break;
	case WM_MBUTTONDOWN:
		SetCapture(hWnd);
		m_mb|=4;
		m_isKeyDown[VK_MBUTTON]=true;
		m_keyHit[VK_MBUTTON]++;
		break;
	case WM_LBUTTONUP:
		ReleaseCapture();
		m_mb&=~1;
		m_isKeyDown[VK_LBUTTON]=false;
		break;
	case WM_RBUTTONUP:
		ReleaseCapture();
		m_mb&=~2;
		m_isKeyDown[VK_RBUTTON]=false;
		break;
	case WM_MBUTTONUP:
		ReleaseCapture();
		m_mb&=~4;
		m_isKeyDown[VK_MBUTTON]=false;
		break;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		m_isKeyDown[wParam&255]=true;
		m_keyHit[wParam&255]++;
		return 0;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		m_isKeyDown[wParam&127]=false;
		break;

	case WM_DESTROY:
		// Tidying up of resources handled in the Renderer destructor.
		PostQuitMessage( 0 );
		return 0;
	case WM_ACTIVATEAPP:
		if (!wParam)
		{
			memset(m_isKeyDown,0,sizeof(m_isKeyDown));
		}
		break;

	case WM_ACTIVATE:
		if( WA_INACTIVE != wParam )
		{
			// Make sure the device is acquired, if we are gaining focus.
		}
		break;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

void InputController::HandleInput(float frameTime)
{
	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );
	while ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );

		if (msg.message==WM_QUIT) {
			Game::GetInstance().SetRunning(false);
			return;
		}
	}

	for ( int i = 0; i < KEY_ARRAY_SIZE; ++i )
	{
		m_wasKeyDown[i] = m_isKeyDown[i];
		m_isKeyDown[i] = false;
	}

	/*
	// Populate the key state from the game system.
	IDiceInvaders::KeyStatus keystate;
	Game::GetInstance().GetSystem().getKeyStatus(keystate);

	bool gameOver = GameState::GetInstance().IsGameOver();

	// We want to process game controls if it's still running ...
	if ( !gameOver )
	{
		GameControls(keystate, frameTime);
	}
	// ... otherwise we listen for space bar to exit the game.
	else
	{
		GameOverScreen(keystate);
	}
	*/
}



/*
void InputController::GameControls(float frameTime)
{
	// Process the key state.
	if ( keystate.left )
	{
		// Move the player in negative X.
		Game::GetInstance().GetPlayer().Move(-1, frameTime);

		Game::GetInstance().GetPlayer().SetSpeedingUp();
	}
	
	if ( keystate.right )
	{
		// Move the player in position X.
		Game::GetInstance().GetPlayer().Move(1, frameTime);

		Game::GetInstance().GetPlayer().SetSpeedingUp();
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
	Vector2 playerPos = Game::GetInstance().GetPlayer().GetPosition();
	
	std::stringstream ss;
	ss << "Player is at: [" << playerPos.x << ", " << playerPos.y << "]";
	std::string pos = ss.str();

	Game::GetInstance().GetSystem().drawText(0, 0, pos.c_str());
#endif
}

void InputController::GameOverScreen(const IDiceInvaders::KeyStatus& keystate)
{
	if ( keystate.fire )
	{
		if ( !m_wasFiring)
		{
			// The game is no longer running.
			Game::GetInstance().SetRunning(false);
		}
	}
}
*/