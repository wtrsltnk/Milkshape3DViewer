#include "CMainFrame.h"
	
MilkshapeModel CMainFrame::m_Model;

/////////////////////////////////////
// Constructors / Destructors      //
/////////////////////////////////////
CMainFrame::CMainFrame()
{
	this->m_dwCreationFlags  = 0L;
	this->m_dwWindowStyle	 = WS_OVERLAPPEDWINDOW;
	this->m_dwExWindowStyle	 = WS_EX_WINDOWEDGE;
	this->m_dwCreationFlags  = SW_SHOW;
	this->m_PosX			 = CW_USEDEFAULT;	
	this->m_PosY			 = CW_USEDEFAULT;	
	this->m_dwCreationWidth  = CW_USEDEFAULT;
	this->m_dwCreationHeight = CW_USEDEFAULT;
	this->m_hbrWindowColor	 = (HBRUSH)(COLOR_WINDOW);
	this->m_hIcon			 = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_GMAX));
	this->m_strWindowTitle	 = "Window Test Application";
	this->m_hMenu			 = NULL;
}


CMainFrame::~CMainFrame()
{
}

/////////////////////////////////////
// Memberfuncties                  //
/////////////////////////////////////
void CMainFrame::Teken( void )
{
	m_Model.draw();
/*	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glBegin(GL_QUADS);
		// Front Face
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		// Back Face
		glNormal3f( 0.0f, 0.0f,-1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		// Top Face
		glNormal3f( 0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		// Bottom Face
		glNormal3f( 0.0f,-1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		// Right face
		glNormal3f( 1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		// Left Face
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glEnd();
*/
}
void CMainFrame::Init( void )
{
	m_Model.reloadTextures();
}

LRESULT CMainFrame::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nWidth, nHeight;

	switch ( uMsg )
	{
	case WM_CREATE:
		m_Model.loadModelData( "Model.ms3d" );

		m_p3DView2 = new COpenGl( m_hInstance, m_hWnd, 0, 0, 100, 100, 2, (DRAW)Teken, (INIT)Init );
		m_p3DView2->create();

		m_p3DView3 = new COpenGl( m_hInstance, m_hWnd, 0, 0, 100, 100, 3, (DRAW)Teken, (INIT)Init );
		m_p3DView3->create();

		m_p3DView = new COpenGl( m_hInstance, m_hWnd, 0, 0, 100, 100, 1, (DRAW)Teken, (INIT)Init );
		m_p3DView->create();
		break;

	case WM_SIZE:
		nWidth = LOWORD(lParam);
		nHeight = HIWORD(lParam);

		m_p3DView->resize(  0,			0,			nHeight,		nHeight );
		m_p3DView2->resize( nHeight,	0,			nWidth-nHeight,	nHeight/2 );
		m_p3DView3->resize( nHeight,	nHeight/2,	nWidth-nHeight,	nHeight/2 );
		break;
	}
	return CWin::MsgProc(hWnd, uMsg, wParam, lParam);
}

