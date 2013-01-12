#include "Projectile.h"

Projectile::Projectile(float xPos, float yPos)
{
	// Initialise the projectile's position.
	m_position = Position(xPos, yPos);
}

Projectile::~Projectile()
{
}