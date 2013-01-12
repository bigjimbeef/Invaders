#ifndef PLAYER_H
#define PLAYER_H

// Include our base class.
#include "Renderable.h"

// Forward declare Game to allow access to Game singleton.
class Game;

class Player : public IRenderable
{
	public:
		// Default ctor.
		Player(float xPos, float yPos);
		~Player();

		// Implement this function to show that this class is not abstract.
		inline void IsAbstract() { /* Do nothing */ }

		// Initialises the player.
		void Init();

		void Update(float frameTime);
		void Render();

		// Used to the move the player around the screen, with the direction
		// indicating the cardinality of the X move:
		//     1 is right, -1 is left.
		void Move(int direction, float elapsedTime);

	private:
		// The current health of the player.
		int m_health;
		static const int STARTING_HEALTH = 3;
		static const int PLAYER_VELOCITY = 160;

};

#endif // PLAYER_H