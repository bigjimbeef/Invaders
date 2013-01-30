#include "PlayerUI.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

PlayerUI::PlayerUI(float xPos, float yPos)
{
	// Initialise the player's position.
	m_position = Position(xPos, yPos);

	// Get a pointer to the player sprite from the Resource manager.
	mp_sprite = ResourceManager::GetPlayerSprite();
}
PlayerUI::~PlayerUI()
{
	mp_sprite = NULL;
}

void PlayerUI::Render()
{
	mp_sprite->draw(static_cast<int>(m_position.x),
					static_cast<int>(m_position.y));
}
