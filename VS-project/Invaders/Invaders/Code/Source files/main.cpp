//-----------------------------------------------------------------------------
// main.cpp 
// The entry point for the game.
//-----------------------------------------------------------------------------

#include <windows.h>
#include <cassert>
#include <cstdio>

#include "Game.h"

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR cmd, INT )
{
	while ( Game::GetInstance().IsRunning() )
	{
		Game::GetInstance().Run();
	}

	return 0;
}
