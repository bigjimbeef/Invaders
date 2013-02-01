#include "Renderer.h"

// Include within .cpp file to match forward declare in .h
#include "Game.h"

// The message handler for the window. This function has to be global.
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	// Pass the actual message handling to our Renderer's 
	// message handler function.
	Game::GetInstance().GetInputController().MessageHandler(hWnd, msg, 
													 wParam, lParam);

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

//-----------------------------------------------------------------------------
// Start of Renderer-specific functionality.

Renderer::Renderer() :
	mp_wc(NULL),
	m_wcName("InvadersWindow"),
	mp_HWND(NULL),
	m_windowName("Invaders"),
	mp_D3D(NULL),
	mp_d3dDevice(NULL),
	mp_VB(NULL),
	m_fullScreen(false),
	m_bgColour(D3DCOLOR_XRGB(100, 149, 237)) // Cornflower blue!
{
	// Register the window class
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
					GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
					m_wcName, NULL };
	mp_wc = new WNDCLASSEX(wc);
	RegisterClassEx( mp_wc );

	RECT r = {0, 0, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT};

	// Determine whether or not to run in fullscreen.
	int style = m_fullScreen ? WS_POPUP : WS_OVERLAPPEDWINDOW;
	style |= WS_VISIBLE;
	AdjustWindowRect(&r,style,false);

	// Create the window.
	mp_HWND = CreateWindow( m_wcName, m_windowName,
		style, 0,0,r.right-r.left,r.bottom-r.top,
		GetDesktopWindow(), NULL, wc.hInstance, NULL );

	if( SUCCEEDED( InitD3D( mp_HWND ) ) )
	{
		// Create the vertex buffer
		if( SUCCEEDED( InitVB() ) )
		{
			SetCursor(LoadCursor(NULL,IDC_ARROW));

			// Show the window
			ShowWindow( mp_HWND, SW_SHOWDEFAULT );
			UpdateWindow( mp_HWND );
		}
	}
}
Renderer::~Renderer()
{
	if ( mp_D3D != NULL )
	{
		mp_D3D->Release();
	}
	if ( mp_d3dDevice != NULL )
	{
		mp_d3dDevice->Release();
	}
	if ( mp_VB != NULL )
	{
		mp_VB->Release();
	}

	UnregisterClass( m_wcName, mp_wc->hInstance );
	delete mp_wc;
	mp_wc = NULL;
}


HRESULT Renderer::InitD3D( HWND hWnd )
{
	// Create the D3D object.
	mp_D3D = Direct3DCreate9( D3D_SDK_VERSION );
	if ( NULL == mp_D3D )
	{
		return E_FAIL;
	}

	// Set up the structure used to create the D3DDevice
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof(d3dpp) );

	d3dpp.Windowed = !m_fullScreen;
	d3dpp.SwapEffect = m_fullScreen
		? D3DSWAPEFFECT_FLIP 
		: D3DSWAPEFFECT_DISCARD;

	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE ;
	d3dpp.BackBufferFormat = m_fullScreen ? D3DFMT_A8R8G8B8 : D3DFMT_UNKNOWN;

	d3dpp.BackBufferWidth = BACKBUFFER_HEIGHT;
	d3dpp.BackBufferHeight = BACKBUFFER_WIDTH;

	d3dpp.FullScreen_RefreshRateInHz = m_fullScreen ? SIXTY_HERTZ : 0;

	// Create the D3DDevice
	if( FAILED( mp_D3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &mp_d3dDevice ) ) )
	{
		return E_FAIL;
	}

	// Device state would normally be set here

	return S_OK;
}

HRESULT Renderer::InitVB()
{
	if( FAILED( mp_d3dDevice->CreateVertexBuffer( 4*sizeof(CUSTOMVERTEX),
		D3DUSAGE_DYNAMIC, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &mp_VB, NULL ) ) )
	{
		return E_FAIL;
	}

	return S_OK;
}

void Renderer::PreRender()
{
	mp_d3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, m_bgColour, 1.0f, 0 );
	mp_d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	mp_d3dDevice->SetRenderState(D3DRS_ZENABLE,false);

	mp_d3dDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	mp_d3dDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	mp_d3dDevice->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_DIFFUSE);
	mp_d3dDevice->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_TEXTURE);
	mp_d3dDevice->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);

	mp_d3dDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);
	mp_d3dDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_TEXTURE);
	mp_d3dDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);

	mp_d3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	mp_d3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	mp_d3dDevice->SetRenderState(D3DRS_LIGHTING,false);
	//mp_d3dDevice->SetTexture(0,NULL);
	mp_d3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);

	D3DVIEWPORT9 vp = { 0,0, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT, 0, 1};
	mp_d3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );

	// Begin the scene
	mp_d3dDevice->BeginScene();
}

void Renderer::PostRender()
{
	mp_d3dDevice->EndScene();

	// Present the backbuffer to the display, rendering the frame's updates.
	mp_d3dDevice->Present( NULL, NULL, NULL, NULL );

	// TODO: Move this to the InputController.
	//memset(m_keyHit,0,sizeof(m_keyHit));

	SetCursor(LoadCursor(NULL,IDC_ARROW));
}