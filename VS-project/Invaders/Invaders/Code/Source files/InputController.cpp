#include "InputController.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

InputController::InputController() :
	m_frameTime(0.0f),	
	m_mb(0)
{
	for( int i = 0; i < KEY_ARRAY_SIZE; ++i )
	{
		m_isKeyDown[i] = false;
		m_wasKeyDown[i] = false;
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
			break;
		case WM_RBUTTONDOWN:
			SetCapture(hWnd);
			m_isKeyDown[VK_RBUTTON]=true;
			m_mb|=2;
			break;
		case WM_MBUTTONDOWN:
			SetCapture(hWnd);
			m_mb|=4;
			m_isKeyDown[VK_MBUTTON]=true;
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

	return NULL;
}

void InputController::HandleInput(float frameTime)
{
	// Set the frame time, for use in message handler above.
	m_frameTime = frameTime;

	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );
	while ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );

		if (msg.message == WM_QUIT) {
			Game::GetInstance().SetRunning(false);
			return;
		}
	}

	// Process the controls for the game.
	GameControls(frameTime);

	// If we're in the second part of the game, we need to send characters.
	if ( GameState::GetInstance().InMainGameMode() )
	{
		SendCharacters();
	}

	for ( int i = 0; i < KEY_ARRAY_SIZE; ++i )
	{
		m_wasKeyDown[i] = m_isKeyDown[i];
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

bool InputController::IsKeyDown(unsigned int key)
{
	if ( key >= KEY_ARRAY_SIZE )
	{
		return false;
	}

	return m_isKeyDown[key];
}
bool InputController::WasKeyDown(unsigned int key)
{
	if ( key >= KEY_ARRAY_SIZE )
	{
		return false;
	}

	return m_wasKeyDown[key];
}

void InputController::GameControls(float frameTime)
{
	// If we're in education mode, we can't control the player.
	if ( GameState::GetInstance().AreEducating() )
	{
		return;
	}

	// Process the key state.
	if ( IsKeyDown(LEFT_ARROW) )
	{
		// Move the player in negative X.
		Game::GetInstance().GetPlayer().Move(
			MOVE_LEFT, frameTime
		);

		//Game::GetInstance().GetPlayer().SetSpeedingUp();
	}

	if ( IsKeyDown(RIGHT_ARROW) )
	{
		// Move the player in position X.
		Game::GetInstance().GetPlayer().Move(
			MOVE_RIGHT, frameTime
		);

		//Game::GetInstance().GetPlayer().SetSpeedingUp();
	}

	if ( IsKeyDown(SPACEBAR) )
	{
		// We can only shoot again if we've let go of the key.
		// Don't really want the player to be able to machine gun fire
		// by just holding down the spacebar!
		if ( !WasKeyDown(SPACEBAR) )
		{
			// Make the player fire a rocket.
			Game::GetInstance().GetPlayer().Fire();
		}
	}
}

char InputController::GetHeldKey()
{
	char heldKey = 0;
	const char* alphabet = "abcdefghijklmnopqrstuvwxyz";

	for ( int i = A_KEY; i < A_KEY + 26; ++i )
	{
		if ( IsKeyDown(i) )
		{
			int index = i - A_KEY;
			heldKey = alphabet[index];
			break;
		}
	}

	return heldKey;
}

void InputController::SendCharacters()
{
	int letter = GetHeldKey();
	if ( letter == 0 )
	{
		return;
	}

	int test = 3;

	// Firstly, we need to look and see if we're in "education" mode
	// (that is, are we locked into typing a word to please the invader)
	//if ( GameState::GetInstance().AreEducating() ) {

	// TODO

	if ( false )
	{

	}
	else
	{
		// In order to prevent keyboard spam, we just look at the first
		// key that the player has held down.

		// Search to see if there are any Projectiles with Words attached.
		Game::GetInstance().GetProjectileManager().SendCharacter(letter);
	}
}

/*
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