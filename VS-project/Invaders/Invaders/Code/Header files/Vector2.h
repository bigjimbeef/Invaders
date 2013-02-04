#ifndef VECTOR2_H
#define VECTOR2_H

//-----------------------------------------------------------------------------
// Vector2.h
// Represents a point in space within the game world where 0,0 is the top left 
// of the screen.
//-----------------------------------------------------------------------------

struct Vector2
{
	// Defaults to being positioned at 0,0.
	Vector2(float xPos = 0, float yPos = 0);
	~Vector2();

	inline bool operator==( Vector2 const& rhs ) {
		return (x == rhs.x) && (y == rhs.y);
	}

	float x;
	float y;
};

#endif // VECTOR2_H