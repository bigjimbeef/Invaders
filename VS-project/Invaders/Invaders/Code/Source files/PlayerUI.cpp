#include "PlayerUI.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

PlayerUI::PlayerUI(float xPos, float yPos)
{
	// Initialise the player's position.
	m_position = Position(xPos, yPos);
}
PlayerUI::~PlayerUI()
{
	mp_sprite = NULL;
}

void PlayerUI::Init()
{
	// Get a pointer to the player sprite from the Resource manager.
	mp_sprite = ResourceManager::GetPlayerSprite();
}

void PlayerUI::Render()
{
	mp_sprite->draw(int(m_position.x), int(m_position.y));
}
