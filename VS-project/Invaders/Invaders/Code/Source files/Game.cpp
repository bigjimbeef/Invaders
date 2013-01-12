#include "Game.h"

Game::Game() :
	m_initialised(0),
	m_gameRunning(1),
	mp_library(0),
	mp_system(0)
{
	// Initialising these variables outside of the initialisation list,
	// as re-ordering the header file could cause problems with initialisation
	// order.
	m_libPath = "DiceInvaders.dll";
	mp_library = new DiceInvadersLib(m_libPath);
}
Game::~Game()
{
	// Tidy up.
	delete mp_library;
}

void Game::Init()
{
	// Initialise the game system from the DiceInvaders library.
	mp_system = mp_library->get();
	mp_system->init(640,480);

	// We have no initialised the game.
	m_initialised = true;
}


void Game::Run()
{
	// If we haven't yet initialised the game ...
	if ( !m_initialised )
	{
		// ... then do so.
		Init();
	}

	// Handle input from the player.
	HandleInput();

	// Update all objects in the game world.
	Update();

	// Render the game world.
	Render();
}


void Game::HandleInput()
{

}

void Game::Update()
{

}

void Game::Render()
{

}

