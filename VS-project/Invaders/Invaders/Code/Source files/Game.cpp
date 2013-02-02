#include "Game.h"

Game::Game() :
	m_initialised(0),
	m_gameRunning(1),
	// m_frequency initialised below.
	m_startTime(0.0f),
	m_totalTime(0.0f),
	m_lastFrameTime(0.0f),
	m_frameRateCapped(true),
	m_frameRateCap(60),
	mp_renderer(NULL),
	mp_player(NULL),
	mp_audioManager(),
	mp_inputController(NULL),
	m_gameSpeedFactor(1.0f)
{
	// Initialise the ResourceManager, which sets the static pointers to
	// the sprites, for use in drawing all IRenderables.
	//ResourceManager::GetInstance();

	// Initialise game frequency ...
	QueryPerformanceFrequency (&m_frequency );
	// ... and game time.
	m_startTime = GetGameTime();
	m_lastFrameTime = m_startTime;

	// Create the Renderer.
	mp_renderer = new Renderer();

	// Create the resource manager, loading all the textures.
	mp_resourceManager = new ResourceManager();

	// Create a new Input Controller.
	mp_inputController = new InputController();

	// Create the player object.
	mp_player = new Player(static_cast<float>(PLAYER_START_X),
						   static_cast<float>(PLAYER_START_Y));

	mp_audioManager = new AudioManager();

	// Seed the random number generator.
	srand( static_cast<unsigned int>(time(NULL)) );

	// Spawn a wave of enemies.
	EnemyManager::GetInstance().SpawnWave();
}
Game::~Game()
{
	delete mp_player;
	mp_player = NULL;

	delete mp_audioManager;
	mp_audioManager = NULL;

	delete mp_inputController;
	mp_inputController = NULL;

	delete mp_resourceManager;
	mp_resourceManager = NULL;

	delete mp_renderer;
	mp_renderer = NULL;
}

// Gets the current time, in milliseconds
float Game::GetGameTime()
{
	LARGE_INTEGER ticks;
	QueryPerformanceCounter( &ticks );

	float gameTime = 
		static_cast<float>((ticks.QuadPart * 1000.0f) / m_frequency.QuadPart);

	return gameTime;
}

void Game::Run()
{
	float currentTime = GetGameTime();
	float frameTime = currentTime - m_lastFrameTime;

	// Update the time since the game started.
	m_totalTime = currentTime - m_startTime;

	// Render the FPS in the window title.
	float fps = 1000.0f / frameTime;
	std::stringstream ss;
	ss << "FPS: " << fps;
	std::string title = ss.str();
	SetWindowText(mp_renderer->GetWindow(), title.c_str());

	if( m_frameRateCapped )
	{
		float dt = 1000.0f / static_cast<float>(m_frameRateCap);

		if (frameTime < dt)
		{
			//Sleep the remaining frame time 
			Sleep( static_cast<DWORD>(dt - frameTime) );
			frameTime = dt;
		}
	}
	
	// Handle input from the player.
	HandleInput(frameTime);

	// We only update and render the game before Game Over.
	if ( !GameState::GetInstance().IsGameOver() )
	{
		//mp_audioManager->PlayMusic();

		// Update all objects in the game world.
		Update(frameTime);

		// Render the game world.
		Render();
	}

	// Render the (rudimentary) UI for the game. Note that this persists
	// even after the game is finished.
	// GameState::GetInstance().RenderUI(frameTime);

	// Cache the elapsed frame time.
	m_lastFrameTime = currentTime;
}


void Game::HandleInput(const float frameTime)
{
	// Farm the input handling out to the input controller.
	mp_inputController->HandleInput(frameTime);
}

void Game::Update(const float frameTime)
{
	
	// Update the player.
	mp_player->Update(frameTime);

	// Update the EnemyManager, which will in turn update the Enemies.
	EnemyManager::GetInstance().Update(frameTime);

	/*
	// Update the ProjectileManager, which will in turn update the projectiles.
	ProjectileManager::GetInstance().Update(frameTime);
	*/

	// Update the audio manager, which will update the play speed of all audio.
	mp_audioManager->Update(frameTime);	
}

void Game::Render()
{
	mp_renderer->PreRender();

	// Render the player.
	mp_player->Render();
	
	// Render the EnemyManager, which will in turn render the Enemies.
	EnemyManager::GetInstance().Render();
	
	/*
	// Render the ProjectileManager, which will in turn render the projectiles.
	ProjectileManager::GetInstance().Render();
	*/

	mp_renderer->PostRender();
}

void Game::SetSpeedFactor(float value)
{
	m_gameSpeedFactor = value;

	// Also set the audio speed factor.
	mp_audioManager->SetPlaybackSpeed(value);
}