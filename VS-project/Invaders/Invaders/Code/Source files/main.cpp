//-----------------------------------------------------------------------------
// main.cpp 
// The entry point for the game.
//-----------------------------------------------------------------------------

#include <windows.h>
#include <cassert>
#include <cstdio>

#include "Game.h"

/*
int APIENTRY WinMain(
	HINSTANCE instance,
	HINSTANCE previousInstance,
	LPSTR commandLine,
	int commandShow)
{
*/
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR cmd, INT )
{
	while ( Game::GetInstance().IsRunning() )
	{
		Game::GetInstance().Run();
	}

	return 0;
}
