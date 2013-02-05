#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

//-----------------------------------------------------------------------------
// ResourceManager.h
// Class which exposes a number of static functions to return pointers to 
// centralised resources. This allows reuse of sprites.
//-----------------------------------------------------------------------------

#include <map>

// Forward declare Game to allow access to Game singleton.
class Game;
struct IDirect3DTexture9;

class ResourceManager
{
	public:
		ResourceManager();
		~ResourceManager();

		static inline IDirect3DTexture9* GetEnemyOneSprite() { return s_enemyOneSprite; }
		static inline IDirect3DTexture9* GetEnemyOneAlt() { return s_enemyOneAltSprite; }
		static inline IDirect3DTexture9* GetEnemyTwoSprite() { return s_enemyTwoSprite; }
		static inline IDirect3DTexture9* GetEnemyTwoAlt() { return s_enemyTwoAltSprite; }
		static inline IDirect3DTexture9* GetEnemyThreeSprite() { return s_enemyThreeSprite; }
		static inline IDirect3DTexture9* GetEnemyThreeAlt() { return s_enemyThreeAltSprite; }
		static inline IDirect3DTexture9* GetPlayerSprite() { return s_playerSprite; }
		static inline IDirect3DTexture9* GetRocketSprite() { return s_rocketSprite; }
		static inline IDirect3DTexture9* GetEnemyProjectileSprite() { return s_enemyProjSprite; }
		static inline IDirect3DTexture9* GetPauseOverlay() { return s_pauseOverlaySprite; }

		// Access the letter map by name.
		IDirect3DTexture9* GetLetterSprite(char letter);

		// Used to safely delete the static pointers to the sprites in the
		// correct order.
		void Destroy();

	private:
		static IDirect3DTexture9* s_enemyOneSprite;
		static IDirect3DTexture9* s_enemyOneAltSprite;
		static IDirect3DTexture9* s_enemyTwoSprite;
		static IDirect3DTexture9* s_enemyTwoAltSprite;
		static IDirect3DTexture9* s_enemyThreeSprite;
		static IDirect3DTexture9* s_enemyThreeAltSprite;
		static IDirect3DTexture9* s_playerSprite;
		static IDirect3DTexture9* s_rocketSprite;
		static IDirect3DTexture9* s_enemyProjSprite;
		static IDirect3DTexture9* s_pauseOverlaySprite;

		static const int NUM_LETTERS;
		std::map<char, IDirect3DTexture9*> m_letterSprites;
};

#endif // RESOURCEMANAGER_H