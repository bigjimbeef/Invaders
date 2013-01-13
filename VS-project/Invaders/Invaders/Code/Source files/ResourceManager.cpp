#include "ResourceManager.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

#define SAFE_DESTROY(x) if (x) { x->destroy(); x = NULL; }

ISprite* ResourceManager::s_enemyOneSprite = NULL;
ISprite* ResourceManager::s_enemyTwoSprite = NULL;
ISprite* ResourceManager::s_playerSprite = NULL;
ISprite* ResourceManager::s_rocketSprite = NULL;
ISprite* ResourceManager::s_bombSprite = NULL;

void ResourceManager::Init()
{
	s_playerSprite = Game::GetInstance().GetSystem().createSprite("data/player.bmp");
	s_enemyOneSprite = Game::GetInstance().GetSystem().createSprite("data/enemy1.bmp");
	s_enemyTwoSprite = Game::GetInstance().GetSystem().createSprite("data/enemy2.bmp");
	s_rocketSprite = Game::GetInstance().GetSystem().createSprite("data/rocket.bmp");
	s_bombSprite = Game::GetInstance().GetSystem().createSprite("data/bomb.bmp");
}

void ResourceManager::Destroy()
{
	SAFE_DESTROY(s_playerSprite);
	SAFE_DESTROY(s_enemyOneSprite);
	SAFE_DESTROY(s_enemyTwoSprite);
	SAFE_DESTROY(s_rocketSprite);
	SAFE_DESTROY(s_bombSprite);
}