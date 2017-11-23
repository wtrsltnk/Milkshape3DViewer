#ifndef CWINDOW_H
#define CWINDOW_H

#include <windows.h>

class Window
{
private:
	HWND			m_hWnd;
	HINSTANCE		m_hInstance;
	HWND			m_hParent;

	DWORD			m_nControlId;
	DWORD			m_PosX;
	DWORD			m_PosY;
	DWORD			m_dwWindowStyle;
	DWORD			m_dwExWindowStyle;
 	DWORD			m_dwCreationFlags;
	DWORD			m_dwCreationHeight;
	DWORD			m_dwCreationWidth;
	CHAR			*m_strWindowTitle;
	HBRUSH			m_hbrWindowColor;
	HICON			m_hIcon;
	HMENU			m_hMenu;
	HACCEL			m_hAccelTable;
public:
	Window( HINSTANCE hInstance, HWND hParent, DWORD nControlId );
	~Window();

	HRESULT create();
	void resize( int nX, int nY, int nWidth, int nHeight );

	LRESULT ClassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void SetHwnd( HWND hWnd ){ m_hWnd = hWnd; };
};

#endif