#include "stdafx.h"
#include "OglContext.h"
#include "GraphPlotter.h"

LRESULT CALLBACK OglProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void OglContext::errorMsg(const char * err)
{
	MessageBoxA(NULL, err, "Error", MB_OK);
}

OglContext::OglContext(HINSTANCE hInstance)
{
	this->hInstance = hInstance;
	//szTitle, MAX_LOADSTRING);
	//LoadStringW(hInstance, IDC_GRAPHPLOTTER, szWindowClass, MAX_LOADSTRING);
}


OglContext::~OglContext()
{
	wglMakeCurrent(NULL, NULL);
	ReleaseDC(this->hWnd, this->hDC);
	wglDeleteContext(this->hRC);
	DestroyWindow(this->hWnd);
}

ATOM OglContext::RegisterWinClass(WNDPROC WndProc)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_DBLCLKS;
	wcex.lpfnWndProc = OglProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = this->hInstance;
	wcex.hIcon = LoadIcon(this->hInstance, MAKEINTRESOURCE(IDI_GRAPHPLOTTER));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_BACKGROUND +1 );
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = this->szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL OglContext::InitInstance(int nCmdShow, UINT16 width, UINT16 height, HWND phwnd)
{
	/*
	this->hWnd = CreateWindowA("test", "test2", WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
		CW_USEDEFAULT, 0, width, height, phwnd, nullptr, this->hInstance, nullptr);
	*/
	
	this->hWnd = CreateWindowW(this->szWindowClass, this->szTitle, WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
		CW_USEDEFAULT, 0, width, height, phwnd, nullptr, this->hInstance, nullptr);
	if (!(this->hWnd))
	{
		this->errorMsg("No hwnd");
		return FALSE;
	}

	ShowWindow(this->hWnd, nCmdShow);
	UpdateWindow(this->hWnd);
	SetParent(this->hWnd, phwnd);

	this->hDC = GetDC(this->hWnd);

	PIXELFORMATDESCRIPTOR pfd;
	/* there is no guarantee that the contents of the stack that become
	the pfd are zeroed, therefore _make sure_ to clear these bits. */
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int pf = ChoosePixelFormat(this->hDC, &pfd);
	if (pf == 0) {
		this->errorMsg("ChoosePixelFormat() failed:  "
			"Cannot find a suitable pixel format.");
		return 0;
	}

	if (SetPixelFormat(this->hDC, pf, &pfd) == FALSE) {
		this->errorMsg("SetPixelFormat() failed:  "
			"Cannot set format specified.");
		return 0;
	}

	//DescribePixelFormat(this->hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	//ReleaseDC(this->hWnd, this->hDC);


	this->hRC = wglCreateContext(this->hDC);
	wglMakeCurrent(this->hDC, this->hRC);

	return TRUE;
}

void
display()
{
	//MessageBoxA(NULL, "display", "Error", MB_OK);
	/* rotate a triangle around */
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2i(0, 1);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2i(-1, -1);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2i(1, -1);
	glEnd();
	//glFlush();
}

LRESULT CALLBACK OglProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		display();
		SwapBuffers(hdc);
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_SIZE:
		glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
		PostMessage(hWnd, WM_PAINT, 0, 0);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}