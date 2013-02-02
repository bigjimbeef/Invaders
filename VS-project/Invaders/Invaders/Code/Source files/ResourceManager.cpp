#include "ResourceManager.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

IDirect3DTexture9* ResourceManager::s_enemyOneSprite = NULL;
IDirect3DTexture9* ResourceManager::s_enemyOneAltSprite = NULL;
IDirect3DTexture9* ResourceManager::s_enemyTwoSprite = NULL;
IDirect3DTexture9* ResourceManager::s_enemyTwoAltSprite = NULL;
IDirect3DTexture9* ResourceManager::s_playerSprite = NULL;
IDirect3DTexture9* ResourceManager::s_rocketSprite = NULL;
IDirect3DTexture9* ResourceManager::s_bombSprite = NULL;

ResourceManager::ResourceManager()
{
	s_playerSprite = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/test.png");
	s_enemyOneSprite = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/jelly_test.png");
	s_enemyTwoSprite = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/crabby_test.png");

	/*
	s_playerSprite =
	    Game::GetInstance().GetSystem().createSprite("data/player.bmp");
	s_enemyOneSprite =
	    Game::GetInstance().GetSystem().createSprite("data/enemy1.bmp");
	s_enemyOneAltSprite =
	    Game::GetInstance().GetSystem().createSprite("data/enemy1_alt.bmp");
	s_enemyTwoSprite =
	    Game::GetInstance().GetSystem().createSprite("data/enemy2.bmp");
	s_enemyTwoAltSprite =
	    Game::GetInstance().GetSystem().createSprite("data/enemy2_alt.bmp");
	s_rocketSprite =
	    Game::GetInstance().GetSystem().createSprite("data/rocket.bmp");
	s_bombSprite =
	    Game::GetInstance().GetSystem().createSprite("data/bomb.bmp");
	*/
}

ResourceManager::~ResourceManager()
{

	// TODO: Unload everything properly!

	/*
	SAFE_DESTROY(s_playerSprite);
	SAFE_DESTROY(s_enemyOneSprite);
	SAFE_DESTROY(s_enemyOneAltSprite);
	SAFE_DESTROY(s_enemyTwoSprite);
	SAFE_DESTROY(s_enemyTwoAltSprite);
	SAFE_DESTROY(s_rocketSprite);
	SAFE_DESTROY(s_bombSprite);
	*/
}