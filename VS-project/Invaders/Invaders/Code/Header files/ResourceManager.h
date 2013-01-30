#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

//-----------------------------------------------------------------------------
// ResourceManager.h
// Class which exposes a number of static functions to return pointers to 
// centralised resources. This allows reuse of sprites.
//-----------------------------------------------------------------------------

// Forward declare Game to allow access to Game singleton.
class Game;
struct ISprite;

class ResourceManager
{
	public:
		static ResourceManager& GetInstance()
		{
			static ResourceManager instance;
			return instance;
		}

		static inline ISprite* GetEnemyOneSprite() { return s_enemyOneSprite; }
		static inline ISprite* GetEnemyOneAltSprite() { return s_enemyOneAltSprite; }
		static inline ISprite* GetEnemyTwoSprite() { return s_enemyTwoSprite; }
		static inline ISprite* GetEnemyTwoAltSprite() { return s_enemyTwoAltSprite; }
		static inline ISprite* GetPlayerSprite() { return s_playerSprite; }
		static inline ISprite* GetRocketSprite() { return s_rocketSprite; }
		static inline ISprite* GetBombSprite() { return s_bombSprite; }

		// Used to safely delete the static pointers to the sprites in the
		// correct order.
		void Destroy();

	private:
		static ISprite* s_enemyOneSprite;
		static ISprite* s_enemyOneAltSprite;
		static ISprite* s_enemyTwoSprite;
		static ISprite* s_enemyTwoAltSprite;
		static ISprite* s_playerSprite;
		static ISprite* s_rocketSprite;
		static ISprite* s_bombSprite;

		// Private default ctor to facilitate Singleton pattern.
		ResourceManager();

		// DO NOT IMPLEMENT
		ResourceManager(ResourceManager const&);
		void operator=(ResourceManager const&);
};

#endif // RESOURCEMANAGER_H