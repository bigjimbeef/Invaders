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
	mp_player = new Player(static_cast<float>(PLAYER_START_X),
						   static_cast<float>(PLAYER_START_Y)
						   );
}
Game::~Game()
{
	// Destroy the game system.
	mp_system->destroy();
	mp_system = 0;

	// Tidy up.
	delete mp_library;
	mp_library = 0;

	delete mp_player;
	mp_player = 0;
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
	if ( !mp_system->update() )
	{
		m_gameRunning = false;
		return;
	}

	// Use the DiceInvaders library to get the elapsed frame time.
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
	ProjectileManager::GetInstance().Update(frameTime);

	// Update the GameStateManager, which holds score, current wave, etc.
}

void Game::Render()
{
	// Render the player.
	mp_player->Render();

	// Render the EnemyManager, which will in turn render the Enemies.

	// Render the ProjectileManager, which will in turn render the projectiles.
	ProjectileManager::GetInstance().Render();

#ifdef _DEBUG
	// Render the debug text, but only in Debug mode.
	// TODO: Maybe reenable Debug.
	// Debug::GetInstance().Render();
#endif
}

