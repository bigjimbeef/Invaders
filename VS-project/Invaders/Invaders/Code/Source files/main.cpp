//-----------------------------------------------------------------------------
// main.cpp 
// The entry point for the game.
//-----------------------------------------------------------------------------

#include <windows.h>
#include <cassert>
#include <cstdio>

#include "Game.h"

int APIENTRY WinMain(
	HINSTANCE instance,
	HINSTANCE previousInstance,
	LPSTR commandLine,
	int commandShow)
{
	while ( Game::GetInstance().IsRunning() )
	{
		Game::GetInstance().Run();
	}

#ifdef MEMLEAK_DEBUGGING
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}
