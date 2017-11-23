#ifndef CMAINFRAME_H
#define CMAINFRAME_H

#define WIN32_LEAN_AND_MEAN

#include "CWin.h"
#include "Window.h"
#include "COpenGl.h"
#include "MilkshapeModel.h"

class CMainFrame : public CWin
{
private:
	COpenGl		*m_p3DView;
	COpenGl		*m_p3DView2;
	COpenGl		*m_p3DView3;
	static MilkshapeModel m_Model;
	HWND h1, h2;
	static void Teken( void );
	static void Init( void );
public:
	CMainFrame();
	~CMainFrame();

	LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);
};

#endif
