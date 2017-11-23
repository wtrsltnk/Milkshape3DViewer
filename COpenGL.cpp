#include "COpenGL.h"
#include <windows.h>
#include <assert.h>

//////////////////////////////////////////////////////////////////
// Koppeling WIN32 -> Klasse
//////////////////////////////////////////////////////////////////
LRESULT CALLBACK GlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	COpenGl *wnd = 0; 

	if(uMsg == WM_NCCREATE) 
	{ 
		// retrieve Window instance from window creation data and associate 
		wnd = reinterpret_cast<COpenGl *>(((LPCREATESTRUCT)lParam)->lpCreateParams); 
		::SetWindowLong(hWnd, GWL_USERDATA, reinterpret_cast<long>(wnd));

		wnd->SetHWND( hWnd );
	} 
	else 
		// retrieve associated Window instance 
		wnd = reinterpret_cast<COpenGl *>(::GetWindowLong(hWnd, GWL_USERDATA)); 

	// call the windows message handler 
	return wnd->ClassProc(hWnd, uMsg, wParam, lParam); 
}

COpenGl::COpenGl( HINSTANCE hInstance, HWND hParent, int nX, int nY, int nWidth, int nHeight, int nId, DRAW lpDraw, INIT lpInit )
{

	this->m_nId				 = nId;
	this->m_hInstance		 = hInstance;
	this->m_hParent			 = hParent;
	this->m_nX				 = nX;
	this->m_nY				 = nY;
	this->m_nWidth			 = nWidth;
	this->m_nHeight			 = nHeight;
	this->draw				 = lpDraw;
	this->init				 = lpInit;

	this->m_hWnd			 = NULL;
	this->m_dwWindowStyle	 = WS_VISIBLE | WS_CHILD;
	this->m_dwExWindowStyle	 = WS_EX_STATICEDGE;
	this->m_dwCreationFlags  = SW_SHOW;
	this->m_hbrWindowColor	 = (HBRUSH)(COLOR_WINDOW+1);
	this->m_hIcon			 = LoadIcon(m_hInstance, (LPCTSTR)IDI_APPLICATION);
	this->m_strWindowTitle	 = "Title";
	this->m_bFocus			 = FALSE;

	m_hHandCur = LoadCursor( m_hInstance, MAKEINTRESOURCE(IDC_HANDJE) );
}

COpenGl::~COpenGl()
{
    purge();
}

LRESULT COpenGl::ClassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	DWORD fwKeys;
	float x, y;

	vecf coordinate = {5.0f, 1.0f, 5.0f};
	vecf origin = {1.0f, 1.0f, 1.0f};
	veci rotation = {45, 45, 45};
	
	switch ( uMsg )
	{

	case WM_SETFOCUS:
	case WM_KILLFOCUS:
		m_bFocus = (uMsg==WM_SETFOCUS);
/*
        SetWindowLong( hWnd, GWL_EXSTYLE, 
			uMsg==WM_SETFOCUS?WS_EX_DLGMODALFRAME:WS_EX_STATICEDGE );
		SetWindowPos( hWnd, NULL, 0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED );
//*/
		InvalidateRect( hWnd, NULL, FALSE );
		break;

	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_LBUTTONDOWN:
		oldx = LOWORD(lParam);
		oldy = HIWORD(lParam);
		oldpx = panx;
		oldpy = pany;
		oldpz = panz;
		oldrx = rotx;
		oldry = roty;
		oldrz = rotz;
		SetCursor( m_hHandCur );
		SetFocus( hWnd );
		break;

	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_LBUTTONUP:
		SetCursor( LoadCursor( NULL, IDC_ARROW ) );
		break;

	case WM_MOUSEMOVE:
		fwKeys = wParam;
		x = LOWORD(lParam);
		y = HIWORD(lParam);

		if ( m_bFocus )
		{
			switch (fwKeys)
			{
			case MK_LBUTTON:
				rotz = oldrz - ( oldx - x );
				rotx = oldrx - ( oldy - y );
				SetCursor( m_hHandCur );
				InvalidateRect( hWnd, NULL, FALSE );
				break;
			case MK_RBUTTON:
				panz = oldpz + ( ( oldy - y ) / 50 );
				SetCursor( m_hHandCur );
				InvalidateRect( hWnd, NULL, FALSE );
				break;
			case MK_MBUTTON:
				panx = oldpx - ( ( oldx - x ) / 30 );
				pany = oldpy + ( ( oldy - y ) / 30 );
				SetCursor( m_hHandCur );
				InvalidateRect( hWnd, NULL, FALSE );
				break;
			case MK_RBUTTON|MK_SHIFT:
				panx = oldpx - ( ( oldx - x ) / 20 );
				pany = oldpy + ( ( oldy - y ) / 20 );
				SetCursor( m_hHandCur );
				InvalidateRect( hWnd, NULL, FALSE );
				break;
			case MK_LBUTTON|MK_SHIFT:
				roty = oldry - ( oldx - x );
				SetCursor( m_hHandCur );
				InvalidateRect( hWnd, NULL, FALSE );
				break;
			}
		}
		break;

	case WM_KEYDOWN:
		switch ( wParam )
		{
		case 49://VK_1:
			SetRenderMode( RENDERMODE_VERTEX );
			break;
		case 50://VK_2:
			SetRenderMode( RENDERMODE_WIREFRAME );
			break;
		case 51://VK_3:
			SetRenderMode( RENDERMODE_FLATSHADE );
			break;
		case 52://VK_4:
			SetRenderMode( RENDERMODE_SMOOTHSHADE );
			break;
		case 53://VK_5:
			SetRenderMode( RENDERMODE_TEXTURED );
			break;
		case 71://G
			m_bGrid = !m_bGrid;
			break;
		}
		InvalidateRect( hWnd, NULL, FALSE );
		break;

	case WM_PAINT:
		drawstart();
		draw();
		draworigin(origin,rotation);
		drawcross(coordinate);
		drawend();
		
		ValidateRect( hWnd, NULL );
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HWND COpenGl::create()
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) GlProc;						// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= m_hInstance;							// Set The Instance
	wc.hIcon			= m_hIcon;								// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= m_hbrWindowColor;						// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGl";								// Set The Class Name


	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		//MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		//return FALSE;											// Return FALSE
	}

	// Create The Window
	if (!(m_hWnd=CreateWindowEx(	this->m_dwExWindowStyle,			// Extended Style For The Window
								"OpenGl",							// Class Name
								"",								// Window Title
								this->m_dwWindowStyle |				// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								m_nX, m_nY,							// Window Position
								m_nWidth,							// Calculate Window Width
								m_nHeight,							// Calculate Window Height
								m_hParent,							// No Parent Window
								NULL,								// No Menu
								m_hInstance,						// Instance
								this)))								// Dont Pass Anything To WM_CREATE
	{
		kill();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		16,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(m_hDC=GetDC(m_hWnd)))						// Did We Get A Device Context?
	{
		kill();										// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(m_hDC,&pfd)))// Did Windows Find A Matching Pixel Format?
	{
		kill();										// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(m_hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		kill();										// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(m_hRC=wglCreateContext(m_hDC)))			// Are We Able To Get A Rendering Context?
	{
		kill();										// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(m_hDC,m_hRC))				// Try To Activate The Rendering Context
	{
		kill();										// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(m_hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(m_hWnd);					// Slightly Higher Priority
	SetFocus(m_hWnd);								// Sets Keyboard Focus To The Window

	initCamara();
	initWindow();

	BuildFont();

	return m_hWnd;
}

void COpenGl::kill()
{
	if (m_hRC)												// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))						// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(m_hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		m_hRC=NULL;											// Set RC To NULL
	}

	if (m_hDC && !ReleaseDC(m_hWnd,m_hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		m_hDC=NULL;											// Set DC To NULL
	}

	if (m_hWnd && !DestroyWindow(m_hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		m_hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",m_hInstance))				// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		m_hInstance=NULL;									// Set hInstance To NULL
	}

	KillFont();
}

void COpenGl::resize( int nX, int nY, int nWidth, int nHeight )
{
	this->m_nX = nX;
	this->m_nY = nY;
	this->m_nWidth = nWidth;
	this->m_nHeight = nHeight;

	MoveWindow( m_hWnd, m_nX, m_nY, m_nWidth, m_nHeight, true );
 
    wglMakeCurrent( m_hDC, m_hRC );

	glViewport(0, 0, m_nWidth, m_nHeight);

	// PROJECTION
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f,(GLfloat)m_nWidth/(GLfloat)m_nHeight,0.01f,300.0f);
	//glOrtho(-0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 100);
}

void COpenGl::purge()
{
    if ( m_hRC )
    {
        wglMakeCurrent( NULL, NULL );
        wglDeleteContext( m_hRC );
    }
    if ( m_hWnd && m_hDC )
    {
        ReleaseDC( m_hWnd, m_hDC );
    }
    m_hWnd = NULL;
    m_hDC = NULL;
    m_hRC = NULL;
}



// OPENGL FUNCTIES

////////////////////////////////////////////////////////////
// Initialize Camara
// Set default values for rotating, translate
////////////////////////////////////////////////////////////
void COpenGl::initCamara( void )
{
	this->rotx				 = -90.0f;
	this->roty				 = 0.0f;
	this->rotz				 = -90.0f;

	this->panx				 = 0.0f;
	this->pany				 = 0.0f;
	this->panz				 = -1.0f;

	this->oldx				 = -1.0f;
	this->oldy				 = -1.0f;

	this->oldrx				 = -1.0f;
	this->oldry				 = -1.0f;
	this->oldrz				 = -1.0f;

	this->oldpx				 = -1.0f;
	this->oldpy				 = -1.0f;
	this->oldpz				 = -1.0f;

	this->m_bGrid			 = true;
}

////////////////////////////////////////////////////////////
// Initialize OpenGL Window
// Set default values for rendermode and background color
////////////////////////////////////////////////////////////
void COpenGl::initWindow( void )
{
	this->m_nRendermode		 = RENDERMODE_TEXTURED;

	this->bgcolor[0]		 = 0.2f;
	this->bgcolor[1]		 = 0.6f;
	this->bgcolor[2]		 = 0.9f;

	this->LightAmbient[0]	 = 1.5f;
	this->LightAmbient[1]	 = 1.5f;
	this->LightAmbient[2]	 = 1.5f;
	this->LightAmbient[3]	 = 1.0f;

	this->LightDiffuse[0]	 = 0.06f;
	this->LightDiffuse[1]	 = 0.06f;
	this->LightDiffuse[2]	 = 0.06f;
	this->LightDiffuse[3]	 = 0.0f;

	this->LightPosition[0]	 = 8.0f;
	this->LightPosition[1]	 = 6.0f;
	this->LightPosition[2]	 = 4.0f;
	this->LightPosition[3]	 = 1.0f;

	init();

	glClearColor( bgcolor[0], bgcolor[1], bgcolor[2], 0.0f );
 
	glLightfv( GL_LIGHT1, GL_AMBIENT, LightAmbient );
	glLightfv( GL_LIGHT1, GL_DIFFUSE, LightDiffuse );
	glLightfv( GL_LIGHT1, GL_POSITION, LightPosition );
	glEnable( GL_LIGHT1 );
}

////////////////////////////////////////////////////////////
// Open draw sequence
// Always call this function first before starting to draw
////////////////////////////////////////////////////////////
void COpenGl::drawstart()
{
    wglMakeCurrent( m_hDC, m_hRC );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// MODELVIEW
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef( panx, pany, panz );
	glRotatef( rotx,1,0,0 );
	glRotatef( roty,0,1,0 );
	glRotatef( rotz,0,0,1 );
	glScalef( 0.1f, 0.1f, 0.1f );
	
	setupRenderMode();
	
	if (m_bGrid)
		drawgrid();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

////////////////////////////////////////////////////////////
// Close draw sequence
// Always call this function after drawing
////////////////////////////////////////////////////////////
void COpenGl::drawend()
{
	SwapBuffers( m_hDC );
}

////////////////////////////////////////////////////////////
// Draw an origin axis
// Draws a red/green/blue origin axis on the given coordinate
////////////////////////////////////////////////////////////
void COpenGl::draworigin( vecf origin, veci rotation )
{	
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glPushMatrix();
	
	if ( origin != 0 )
		glTranslatef( origin[0], origin[1], origin[2] );

	if ( rotation != 0 ) {
		glRotatef( rotation[0],1,0,0 );
		glRotatef( rotation[1],0,1,0 );
		glRotatef( rotation[2],0,0,1 );
	}

	glBegin(GL_LINES);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0, 0, 0); glVertex3f(2.0f, 0, 0);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0, 0, 0); glVertex3f(0, 2.0f, 0);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0, 0, 0); glVertex3f(0, 0, 2.0f);
	glEnd();

	glPopMatrix();
}

////////////////////////////////////////////////////////////
// Draw grid
// draws a nice grid, according to the zoom
////////////////////////////////////////////////////////////
void COpenGl::drawgrid()
{
	glDisable(GL_LIGHTING);
	glPushMatrix();

	int stap, begin, eind, zoom;
	
	stap = 1;
	begin = -16;
	eind = 16;
	zoom = -panz + 2;

	glScalef(zoom/2, zoom/2, zoom/2);

	for ( int i = begin; i <= eind; i+=stap )
	{
		if ( i%4 == 0 )
			glColor3f(0.1f, 0.4f, 0.7f);
		else
			glColor3f(0.5f, 0.5f, 0.5f);

		glBegin(GL_LINES);
			glVertex3f(i, begin, 0);
			glVertex3f(i, eind, 0);
		glEnd();

		glBegin(GL_LINES);
			glVertex3f(begin, i, 0);
			glVertex3f(eind, i, 0);
		glEnd();
	}

	glPopMatrix();
}

////////////////////////////////////////////////////////////
// Draw cross
// draws a cros to point out a coordinate in the scene
////////////////////////////////////////////////////////////
void COpenGl::drawcross( vecf coordinate )
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glPushMatrix();
	
	if ( coordinate != 0 )
		glTranslatef( coordinate[0], coordinate[1], coordinate[2] );

	glBegin(GL_LINES);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-1, 0, 0); glVertex3f(1.0f, 0, 0);
		glVertex3f(0, -1.0f, 0); glVertex3f(0, 1.0f, 0);
		glVertex3f(0, 0, -1.0f); glVertex3f(0, 0, 1.0f);
	glEnd();

	glPopMatrix();
}

////////////////////////////////////////////////////////////
// Rotate viewpoint
// Call this function to rotate the viewpoint
////////////////////////////////////////////////////////////
void COpenGl::rotate( float fRotX, float fRotY, float fRotZ )
{
	this->rotx += fRotX;
	this->roty += fRotY;
	this->rotz += fRotZ;
}

////////////////////////////////////////////////////////////
// Translate viewpoint
// Call this function to translate the viewpoint
////////////////////////////////////////////////////////////
void COpenGl::move( float fPanX, float fPanY, float fPanZ )
{
	this->panx += fPanX;
	this->pany += fPanY;
	this->panz += fPanZ;
}

////////////////////////////////////////////////////////////
// Setup rendermode
// Modes are RENDERMODE_VERTEX, _WIREFRAME, _FLATSHADE,
// _SMOOTHSHADE & _TEXTURED
////////////////////////////////////////////////////////////
void COpenGl::setupRenderMode( void )
{
	switch ( m_nRendermode )
	{
	case RENDERMODE_VERTEX:
		glPointSize( 3 );

		glPolygonMode ( GL_FRONT_AND_BACK, GL_POINT );
		glDisable (GL_TEXTURE_2D);
		glShadeModel ( GL_SMOOTH );
		break;
	case RENDERMODE_WIREFRAME:
		glLineWidth( 1 );

		glPolygonMode ( GL_FRONT_AND_BACK, GL_LINE );
		glDisable (GL_TEXTURE_2D);
		glShadeModel ( GL_SMOOTH );
		break;
	case RENDERMODE_FLATSHADE:

		glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );
		glDisable (GL_TEXTURE_2D);
		glShadeModel ( GL_FLAT );
		break;
	case RENDERMODE_SMOOTHSHADE:

		glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );
		glDisable (GL_TEXTURE_2D);
		glShadeModel ( GL_SMOOTH );
		break;
	case RENDERMODE_TEXTURED:

		glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );
		glEnable (GL_TEXTURE_2D);
		glShadeModel ( GL_SMOOTH );
		break;
	}
}

////////////////////////////////////////////////////////////
// Set rendermode
// Change the rendermode from outside teh class
////////////////////////////////////////////////////////////
void COpenGl::SetRenderMode( int nMode )
{
	if ( nMode <= RENDERMODE_TEXTURED && nMode >= RENDERMODE_VERTEX )
	{
		this->m_nRendermode = nMode;
	}
}

void COpenGl::BuildFont(void)								// Build Our Bitmap Font
{
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	base = glGenLists(96);								// Storage For 96 Characters

	font = CreateFont(	-24,							// Height Of Font
						0,								// Width Of Font
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						FW_BOLD,						// Font Weight
						FALSE,							// Italic
						FALSE,							// Underline
						FALSE,							// Strikeout
						ANSI_CHARSET,					// Character Set Identifier
						OUT_TT_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						ANTIALIASED_QUALITY,			// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
						"Verdana");						// Font Name

	oldfont = (HFONT)SelectObject(m_hDC, font);           // Selects The Font We Want
	wglUseFontBitmaps(m_hDC, 32, 96, base);				// Builds 96 Characters Starting At Character 32
	SelectObject(m_hDC, oldfont);							// Selects The Font We Want
	DeleteObject(font);									// Delete The Font
}

GLvoid COpenGl::KillFont(GLvoid)									// Delete The Font List
{
	glDeleteLists(base, 96);							// Delete All 96 Characters
}

GLvoid COpenGl::glPrint(const char *fmt, ...)					// Custom GL "Print" Routine
{
	char		text[1024];
	va_list		ap;

	if (fmt == NULL)
		return;

	va_start(ap, fmt);							
	vsprintf(text, fmt, ap);
	va_end(ap);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glPushMatrix();
	
	glOrtho(-1,1,-1,1,1,10);

	glListBase(base - ' ');

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glCallLists(strlen(text), GL_BYTE, text);

	glPopMatrix();
}

void COpenGl::SetError( char *txt, ...)
{
	char		text[1024];
	va_list		ap;

	if (txt == NULL)
		return;

	va_start(ap, txt);							
	vsprintf(text, txt, ap);
	va_end(ap);

	OutputDebugString("\n");
	OutputDebugString(text);
}
