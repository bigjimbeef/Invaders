#include "ResourceManager.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

// Initialise here as they're static pointers and therefore can't be init'd in
// the initialisation list of the ctor.
IDirect3DTexture9* ResourceManager::s_enemyOneSprite = NULL;
IDirect3DTexture9* ResourceManager::s_enemyOneAltSprite = NULL;
IDirect3DTexture9* ResourceManager::s_enemyTwoSprite = NULL;
IDirect3DTexture9* ResourceManager::s_enemyTwoAltSprite = NULL;
IDirect3DTexture9* ResourceManager::s_enemyThreeSprite = NULL;
IDirect3DTexture9* ResourceManager::s_enemyThreeAltSprite = NULL;
IDirect3DTexture9* ResourceManager::s_playerSprite = NULL;
IDirect3DTexture9* ResourceManager::s_rocketSprite = NULL;
IDirect3DTexture9* ResourceManager::s_enemyProjSprite = NULL;
IDirect3DTexture9* ResourceManager::s_pauseOverlaySprite = NULL;

ResourceManager::ResourceManager()
{
	s_playerSprite = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/player.png");
	s_enemyOneSprite = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/jelly.png");
	s_enemyOneAltSprite = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/jelly_alt.png");
	s_enemyTwoSprite = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/crabby.png");
	s_enemyTwoAltSprite = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/crabby_alt.png");
	s_enemyThreeSprite = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/octo.png");
	s_enemyThreeAltSprite = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/octo_alt.png");
	s_enemyProjSprite =
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/enemybomb1.png");
	s_rocketSprite =
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/rocket.png");
	s_pauseOverlaySprite =
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/paused.png");


	// Create the name:letter texture map.
	m_letterSprites = std::map<char, IDirect3DTexture9*>();
	m_letterSprites['a'] = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/Letters/alet.png");
	m_letterSprites['b'] = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/Letters/blet.png");
	m_letterSprites['c'] = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/Letters/clet.png");
	m_letterSprites['d'] = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/Letters/dlet.png");
	m_letterSprites['e'] = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/Letters/elet.png");
	m_letterSprites['f'] = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/Letters/flet.png");
	m_letterSprites['g'] = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/Letters/glet.png");
	m_letterSprites['h'] = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/Letters/hlet.png");
	m_letterSprites['i'] = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/Letters/ilet.png");
	m_letterSprites['j'] = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/Letters/jlet.png");
	m_letterSprites['k'] = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/Letters/klet.png");
	m_letterSprites['l'] = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/Letters/llet.png");
	m_letterSprites['m'] = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/Letters/mlet.png");
	m_letterSprites['n'] = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/Letters/nlet.png");
	m_letterSprites['o'] = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/Letters/olet.png");
	m_letterSprites['p'] = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/Letters/plet.png");
	m_letterSprites['q'] = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/Letters/qlet.png");
	m_letterSprites['r'] = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/Letters/rlet.png");
	m_letterSprites['s'] = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/Letters/slet.png");
	m_letterSprites['t'] = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/Letters/tlet.png");
	m_letterSprites['u'] = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/Letters/ulet.png");
	m_letterSprites['v'] = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/Letters/vlet.png");
	m_letterSprites['w'] = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/Letters/wlet.png");
	m_letterSprites['x'] = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/Letters/xlet.png");
	m_letterSprites['y'] = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/Letters/ylet.png");
	m_letterSprites['z'] = 
		Game::GetInstance().GetRenderer().LoadSprite("Code/Resource files/Images/Letters/zlet.png");
}

ResourceManager::~ResourceManager()
{

	// TODO: Unload everything properly!

}

IDirect3DTexture9* ResourceManager::GetLetterSprite(char letter)
{
	IDirect3DTexture9* p_letter = NULL;

	std::map<char, IDirect3DTexture9*>::iterator it;
	it = m_letterSprites.find(letter);
	if ( it != m_letterSprites.end() )
	{
		p_letter = it->second;
	}

	return p_letter;
}
