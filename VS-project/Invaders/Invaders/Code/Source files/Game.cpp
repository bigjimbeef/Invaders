#include "Game.h"

Game::Game() :
	m_initialised(0),
	m_gameRunning(1),
	m_lastTime(0.0f),
	mp_library(0),
	mp_system(0),
	mp_player(0)
{
	// Initialising these variables outside of the initialisation list,
	// as re-ordering the header file could cause problems with initialisation
	// order.
	m_libPath = "DiceInvaders.dll";
	mp_library = new DiceInvadersLib(m_libPath);

	// Create the player object.
	mp_player = new Player(PLAYER_START_X, PLAYER_START_Y);
}
Game::~Game()
{
	// Destroy the game system and tidy its memory.
	mp_system->destroy();
	delete mp_system;

	// Tidy up.
	delete mp_library;
}

void Game::Init()
{
	// Initialise the game system from the DiceInvaders library.
	mp_system = mp_library->get();
	mp_system->init(SCREEN_WIDTH, SCREEN_HEIGHT);

	// Initialise the player.
	mp_player->Init();

	// Initialise the EnemyManager, which will initialise the Enemies.

	// Initialise the ProjectileManager, allowing access to projectiles.

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

	// Update the game world, which displays all changes that were made in the
	// previous iteration of this function loop.

	// TODO: We need to exit if this update function returns false!
	mp_system->update();

	float newTime = mp_system->getElapsedTime();
	float frameTime = newTime - m_lastTime;

	// Handle input from the player.
	HandleInput(frameTime);

	// Update all objects in the game world.
	Update(frameTime);

	// Render the game world.
	Render();

	// Cache the elapsed frame time.
	m_lastTime = newTime;
}


void Game::HandleInput(float frameTime)
{
	// Farm the input handling out to the input controller.
	InputController::GetInstance().HandleInput(frameTime);
}

void Game::Update(float frameTime)
{
	// Update the player.
	mp_player->Update(frameTime);

	// Update the EnemyManager, which will in turn update the Enemies.

	// Update the ProjectileManager, which will in turn update the projectiles.

	// Update the GameStateManager, which holds score, current wave, etc.
}

void Game::Render()
{
	// Render the player.
	mp_player->Render();

	// Render the EnemyManager, which will in turn render the Enemies.

	// Render the ProjectileManager, which will in turn render the projectiles.
}

