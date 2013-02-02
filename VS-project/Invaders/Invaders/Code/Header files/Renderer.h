#ifndef RENDERER_H
#define RENDERER_H

//-----------------------------------------------------------------------------
// Renderer.h
// A simple renderer using DirectX to manage and draw sprites.
//-----------------------------------------------------------------------------

#include "Windows.h"
#include <d3d9.h>
#include <d3dx9.h>

// TODO: Are these ok?
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

// A structure for our custom vertex type
struct CUSTOMVERTEX
{
	FLOAT x, y, z, rhw; // The transformed position for the vertex
	DWORD color;        // The vertex color
	float u,v;
};

// Forward declare Game to allow access to Game singleton.
class Game;

class Renderer
{
	public:
		Renderer();
		~Renderer();

		// Initialise Direct3D.
		HRESULT InitD3D(HWND hWnd);
		// Initialise our vertex buffer.
		HRESULT InitVB();

		// Renderer internal handler for messages to the game window.
		LRESULT MessageHandler(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);

		// Responsible for setting up each frame for rendering.
		void PreRender();
		// Presents the backbuffer contents to the display.
		void PostRender();

		// Loads a sprite into memory.
		IDirect3DTexture9* LoadSprite(const char *fname);
		// Set the current texture we're drawing with.
		void SetCurrentTexture(IDirect3DTexture9* tex );

		// Draw a specific sprite.
		void DrawSprite(IDirect3DTexture9* sprite, float xcentre, 
						float ycentre, int xsize, int ysize,
						float angle = 0.0f, DWORD colour = 0xFFFFFFFF);

		static inline DWORD GetColour(int r, int g, int b) 
			{ return D3DCOLOR_XRGB(r,g,b); }

		//---------------------------------------------------------------------
		// Accessors
		inline HWND GetWindow() const { return mp_HWND; }

	private:
		// Used to determine X,Y coords with rotation. The factor of two exists
		// in order to more easily allow use of screen space coordinates.
		inline float GetXPos(float xMid, int xSize, 
							 int ySize, float cosine, float sine)
			{ return (xMid + (xSize * cosine) + (ySize * sine)) / 2.f; }

		inline float GetYPos(float yMid, int xSize, 
							 int ySize, float cosine, float sine) 
			{ return (yMid + (ySize * cosine) - (xSize * sine)) / 2.f; }


		// A pointer to our window class.
		WNDCLASSEX* mp_wc;
		// The name used for the window class.
		const char* m_wcName;

		// Our window handle.
		HWND mp_HWND;
		// The name of our actual game window.
		const char* m_windowName;

		// Used to create the D3DDevice
		LPDIRECT3D9 mp_D3D;
		// Our rendering device
		LPDIRECT3DDEVICE9 mp_d3dDevice;
		// Our vertex buffer. Probably won't hold any tea2.
		LPDIRECT3DVERTEXBUFFER9 mp_VB;

		bool m_fullScreen;

		// The background colour we clear to.
		DWORD m_bgColour;

		static const int BACKBUFFER_WIDTH = 640;
		static const int BACKBUFFER_HEIGHT = 480;
		static const int SIXTY_HERTZ = 60;
};

#endif // RENDERER_H