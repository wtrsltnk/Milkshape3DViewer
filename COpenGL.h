#ifndef COPENGL_H
#define COPENGL_H

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "CControl.h"
#include "resource.h"

enum Rendermodes
{
	RENDERMODE_VERTEX,
	RENDERMODE_WIREFRAME,
	RENDERMODE_FLATSHADE,
	RENDERMODE_SMOOTHSHADE,
	RENDERMODE_TEXTURED,
};

enum Viewmodes
{
	VIEWMODE_LEFT,
	VIEWMODE_RIGHT,
	VIEWMODE_TOP,
	VIEWMODE_BOTTOM,
	VIEWMODE_FRONT,
	VIEWMODE_BACK,
	VIEWMODE_PERSPECTIVE
};

typedef VOID (CALLBACK* DRAW)();
typedef VOID (CALLBACK* INIT)();

typedef float vecf[3];
typedef int veci[3];

class COpenGl
{
public:

    COpenGl( HINSTANCE hInstance, HWND hParent, int nX, int nY, int nWidth, int nHeight, int nId, DRAW lpDraw, INIT lpInit );

    ~COpenGl();

	HWND create( void );
	void kill( void );
    void purge( void );
	void resize( int, int, int, int );
	
	void BuildFont( void );
	void KillFont( void );
	void glPrint(const char *fmt, ...);

	void drawstart( void );
	void drawend( void );

	void draworigin( vecf origin, veci rotation );
	void drawgrid( void );
	void drawcross( vecf coordinate );

	void rotate( float, float, float );
	void move( float, float, float );
	
	LRESULT ClassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void SetHWND( HWND hWnd ) { m_hWnd = hWnd; };

	void SetRenderMode( int nMode );

private:
	void SetError( char *txt, ... );

	void initCamara( void );
	void initWindow( void );

	DRAW								draw;
	INIT								init;

	int									base;

	HWND								m_hWnd;
	HWND								m_hParent;
	HINSTANCE							m_hInstance;
    HDC									m_hDC;
    HGLRC								m_hRC;
	int									m_nX, m_nY, m_nWidth, m_nHeight;
	HICON								m_hHandCur;
	BOOL								m_bFocus;

	DWORD								m_nId;
	DWORD								m_dwWindowStyle;
	DWORD								m_dwExWindowStyle;
 	DWORD								m_dwCreationFlags;
	CHAR								*m_strWindowTitle;
	HBRUSH								m_hbrWindowColor;
	HICON								m_hIcon;

	float								oldx, oldy;
	float								oldpx, oldpy, oldpz;
	float								panx, pany, panz;
	float								oldrx, oldry, oldrz;
	float								rotx, roty, rotz;

	int									m_nRendermode;
	int									m_nViewmode;
	bool								m_bGrid;
	bool								m_bRender;

	float								bgcolor[3];
	float								txtcolor[3];

	float								LightAmbient[4];
	float								LightDiffuse[4];
	float								LightPosition[4];

	void setupRenderMode( void );
};

#endif
