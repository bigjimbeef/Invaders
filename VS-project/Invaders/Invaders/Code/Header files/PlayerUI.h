#ifndef PLAYERUI_H
#define PLAYERUI_H

//-----------------------------------------------------------------------------
// PlayerUI.h
// This little class exists just to render the player's lives in the UI.
//-----------------------------------------------------------------------------

// Include our base class.
#include "Renderable.h"

// Forward declare Game to allow access to Game singleton.
class Game;

class PlayerUI : public IRenderable
{
	public:
		PlayerUI(float xPos = 0.0f, float yPos = 0.0f);
		virtual ~PlayerUI();

		virtual void Init();
		virtual void Render();

		// Implement this function to show that this class is not abstract.
		inline void IsAbstract() { /* Do nothing */ }
};

#endif // PLAYERUI_H