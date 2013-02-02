#ifndef ENEMYPROJECTILE_H
#define ENEMYPROJECTILE_H

//-----------------------------------------------------------------------------
// EnemyProjectile.h
// Is a projectile which enemies fire. It can only collide with the player,
// which will destroy the projectile, and decrement the player's lives.
//-----------------------------------------------------------------------------

// Include our base class.
#include "Projectile.h"
#include "Word.h"

// Forward declare Game to allow access to Game singleton.
class Game;
class Enemy;

class EnemyProjectile : public IProjectile
{
	public:
		EnemyProjectile(Enemy& projOwner);
		virtual ~EnemyProjectile();

		virtual void Update(float frameTime);
		virtual void Render();

		// Implement this function to show that this class is not abstract.
		inline void IsAbstract() { /* Do nothing */ }

		// Override the base class function.
		virtual bool IsEnemyProjectile() const { return true; }

		//---------------------------------------------------------------------
		// Accessors

		inline bool IsAlive() const { return m_alive; }
		inline void Kill() { m_alive = false; }
		inline Enemy& GetOwner() const { return m_projOwner; }
		inline Word* GetWord() const { return mp_word; }

	private:
	    // The enemy that fired this projectile.
	    Enemy& m_projOwner;

		// Used in the second mode, this will make this into a "letter" 
		// projectile
		Word* mp_word;

		//static const int PROJ_VELOCITY = 225;
		static const int PROJ_VELOCITY = 150;

	protected:
		bool m_alive;
};

#endif // ENEMYPROJECTILE_H
