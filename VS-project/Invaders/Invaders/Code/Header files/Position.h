#ifndef POSITION_H
#define POSITION_H

//-----------------------------------------------------------------------------
// Position.h
// Represents a point in space within the game world where 0,0 is the top left 
// of the screen.
//-----------------------------------------------------------------------------

struct Position
{
	// Defaults to being positioned at 0,0.
	Position(float xPos = 0, float yPos = 0);
	~Position();

	float x;
	float y;
};

#endif // POSITION_H