#include "Window.h"
#include <stdio.h>

LRESULT CALLBACK StaticProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Window *wnd = 0; 

	if(uMsg == WM_NCCREATE) 
	{ 
		// retrieve Window instance from window creation data and associate 
		wnd = reinterpret_cast<Window *>(((LPCREATESTRUCT)lParam)->lpCreateParams); 
		::SetWindowLong(hWnd, GWL_USERDATA, reinterpret_cast<long>(wnd));
		wnd->SetHwnd(hWnd);
	} 
	else 
		// retrieve associated Window instance 
		wnd = reinterpret_cast<Window *>(::GetWindowLong(hWnd, GWL_USERDATA)); 

	// call the windows message handler 
	return wnd->ClassProc(hWnd, uMsg, wParam, lParam); 
}

Window::Window( HINSTANCE hInstance, HWND hParent, DWORD nControlId )
{
	this->m_hInstance = hInstance;
	this->m_hParent = hParent;

	this->m_nControlId		 = nControlId;
	this->m_hWnd			 = NULL;
	this->m_dwCreationFlags  = 0L;
	this->m_dwWindowStyle	 = WS_VISIBLE | WS_CHILD;
	this->m_dwExWindowStyle	 = WS_EX_STATICEDGE;
	this->m_dwCreationFlags  = SW_SHOW;
	this->m_PosX			 = 100;	
	this->m_PosY			 = 100;	
	this->m_dwCreationWidth  = 100;
	this->m_dwCreationHeight = 100;
	this->m_hbrWindowColor	 = (HBRUSH)(COLOR_WINDOW);
	this->m_hIcon			 = LoadIcon(m_hInstance, (LPCTSTR)IDI_APPLICATION);
	this->m_strWindowTitle	 = "Skelet Programma HI Blok 1.4";
	this->m_hMenu			 = NULL; 
}

Window::~Window()
{
}

HRESULT Window::create()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)StaticProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= m_hInstance;
	wcex.hIcon			= m_hIcon;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= m_hbrWindowColor;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= "WWindow";
	wcex.hIconSm		= NULL;

	::RegisterClassEx(&wcex);

	m_hWnd = ::CreateWindowEx(m_dwExWindowStyle,"WWindow", m_strWindowTitle, m_dwWindowStyle,
	  m_PosX, m_PosY, m_dwCreationWidth, m_dwCreationHeight, m_hParent, (HMENU)m_nControlId, m_hInstance, this);

	DWORD err = GetLastError();

	if (!m_hWnd)
	{
	  return FALSE;
	}

	::ShowWindow(m_hWnd, m_dwCreationFlags);
	::UpdateWindow(m_hWnd);

	return TRUE;
}

void Window::resize( int nX, int nY, int nWidth, int nHeight )
{
	::MoveWindow( m_hWnd, nX, nY, nWidth, nHeight, true );
}

LRESULT Window::ClassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg )
	{
	case WM_CREATE:
		this->m_hWnd = hWnd;
		break;

	case WM_LBUTTONDOWN:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}