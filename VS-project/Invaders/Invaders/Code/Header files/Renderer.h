#ifndef RENDERER_H
#define RENDERER_H

//-----------------------------------------------------------------------------
// Renderer.h
// A simple renderer using DirectX to manage and draw sprites.
//-----------------------------------------------------------------------------

#include <d3d9.h>
#include <d3dx9.h>

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

		//---------------------------------------------------------------------
		// Accessors
		inline HWND GetWindow() const { return mp_HWND; }

	private:
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
		LPDIRECT3DDEVICE9 mp_d3dDevice ;
		// Our vertex buffer. Probably won't hold any tea2.
		LPDIRECT3DVERTEXBUFFER9 mp_VB;

		/*
		// Which keys are down?
		bool m_keyDown[256];
		// How many times have we hit each key?
		int m_keyHit[256];
		// Is the mouse button pressed?
		int m_mb;
		*/

		bool m_fullScreen;

		// The background colour we clear to.
		DWORD m_bgColour;

		static const int BACKBUFFER_WIDTH = 800;
		static const int BACKBUFFER_HEIGHT = 600;
		static const int SIXTY_HERTZ = 60;
};

#endif // RENDERER_H