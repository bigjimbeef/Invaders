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
	// delete mp_sprite;
}

void PlayerUI::Init()
{
	// Create the sprite for the player.
	mp_sprite = Game::GetInstance().GetSystem().createSprite("data/player.bmp");
}
