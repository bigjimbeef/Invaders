#include "Game.h"

Game::Game() :
	m_initialised(0),
	m_gameRunning(1),
	m_lastTime(0.0f),
	mp_library(NULL),
	mp_libPath("DiceInvaders.dll"),
	mp_system(NULL),
	mp_renderer(NULL),
	mp_player(NULL),
	m_gameSpeedFactor(1.0f)
{
	/*
	// Initialising this variables outside of the initialisation list,
	// as re-ordering the header file could cause problems with initialisation
	// order.
	mp_library = new DiceInvadersLib(mp_libPath);

	// Initialise the game system from the DiceInvaders library.
	mp_system = mp_library->get();
	mp_system->init(SCREEN_WIDTH, SCREEN_HEIGHT);

	// Initialise the ResourceManager, which sets the static pointers to
	// the sprites, for use in drawing all IRenderables.
	ResourceManager::GetInstance();
	*/

	// Create the Renderer.
	mp_renderer = new Renderer();

	return;

	// Create the player object.
	mp_player = new Player(static_cast<float>(PLAYER_START_X),
						   static_cast<float>(PLAYER_START_Y));

	mp_audioManager = new AudioManager();

	// Spawn a wave of enemies.
	EnemyManager::GetInstance().SpawnWave();
}
Game::~Game()
{
	delete mp_player;
	mp_player = NULL;

	delete mp_audioManager;
	mp_audioManager = NULL;

	delete mp_renderer;
	mp_renderer = NULL;

	/*
	ResourceManager::GetInstance().Destroy();

	// Destroy the game system.
	mp_system->destroy();
	mp_system = NULL;

	delete mp_library;
	mp_library = NULL;
	*/
}

// TODO: Use this to clamp to 60fps
/*
int	Game::CalculateFrameTime() // ...since start of program
{
	QueryPerformanceCounter(&m_outStartTime);
	QueryPerformanceFrequency(&m_outFreq);

	LARGE_INTEGER outPerfCount;
	QueryPerformanceCounter(&outPerfCount);
	return ((outPerfCount.QuadPart-starttime.QuadPart)*1000) / m_outFreq.QuadPart;
}
*/

void Game::Run()
{
	mp_renderer->PreRender();

	mp_renderer->PostRender();


	return;

	// Update the game world, which displays all changes that were made in the
	// previous iteration of this function loop.
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

	// We only update and render the game before Game Over.
	if ( !GameState::GetInstance().IsGameOver() )
	{
		mp_audioManager->PlayMusic();

		// Update all objects in the game world.
		Update(frameTime);

		// Render the game world.
		Render();
	}

	// Render the (rudimentary) UI for the game. Note that this persists
	// even after the game is finished.
	GameState::GetInstance().RenderUI(frameTime);

	// Cache the elapsed frame time.
	m_lastTime = newTime;
}


void Game::HandleInput(const float frameTime)
{
	// Farm the input handling out to the input controller.
	InputController::GetInstance().HandleInput(frameTime);
}

void Game::Update(const float frameTime)
{
	// Update the player.
	mp_player->Update(frameTime);

	// Update the EnemyManager, which will in turn update the Enemies.
	EnemyManager::GetInstance().Update(frameTime);

	// Update the ProjectileManager, which will in turn update the projectiles.
	ProjectileManager::GetInstance().Update(frameTime);

	// Update the audio manager, which will update the play speed of all audio.
	mp_audioManager->Update(frameTime);
}

void Game::Render()
{
	// Render the player.
	mp_player->Render();

	// Render the EnemyManager, which will in turn render the Enemies.
	EnemyManager::GetInstance().Render();

	// Render the ProjectileManager, which will in turn render the projectiles.
	ProjectileManager::GetInstance().Render();
}

void Game::SetSpeedFactor(float value)
{
	m_gameSpeedFactor = value;

	// Also set the audio speed factor.
	mp_audioManager->SetPlaybackSpeed(value);
}