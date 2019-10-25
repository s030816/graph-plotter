#include "stdafx.h"
#include "Controls.h"
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
	if (this->cntrls)
	{
		delete this->cntrls;
		this->cntrls = nullptr;
	}
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
	this->hWnd = CreateWindowW(this->szWindowClass, this->szTitle, 
		WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
		CW_USEDEFAULT, 0, width, height, phwnd, nullptr, this->hInstance, nullptr);

	this->width = width;
	this->height = height;
	this->phWnd = phwnd;

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
		this->errorMsg("ChoosePixelFormat error: 83");
		return 0;
	}

	if (SetPixelFormat(this->hDC, pf, &pfd) == FALSE) {
		this->errorMsg("SetPixelFormat error:  89");
		return 0;
	}

	DescribePixelFormat(this->hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	//ReleaseDC(this->hWnd, this->hDC);


	this->hRC = wglCreateContext(this->hDC);
	wglMakeCurrent(this->hDC, this->hRC);

	return TRUE;
}

void OglContext::display(void)
{
	//MessageBoxA(NULL, "display", "Error", MB_OK);
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2i(-1, 0);
	glVertex2i(1, 0);

	glVertex2i(0, 1);
	glVertex2i(0, -1);
	glEnd();

	glPointSize(1.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 0.0f, 1.0f);
	/*
	glVertex2f(0.0f, 0.0f);
	glVertex2f(0.005f, 0.005f);
	glVertex2f(0.01f, 0.01f);
	glVertex2f(0.015f, 0.015f);
	*/
	for (auto& i : this->graph)
	{
		glVertex2f(i.first, i.second);
	}
	glEnd();
	//PostMessage(hWnd, WM_PAINT, 0, 0);
	//glFlush();
}

void OglContext::initGraph(void)
{
	GLfloat i = -(this->width/2.0f);
	GLfloat limit = 1.0f + 1.0f / this->width;
	GLfloat increment = 1.0f / this->width;
	this->graph.clear();
	for (GLfloat x = -1.0f; x < 1.005f; x += increment, i += 1.0f)
	{
		this->graph.emplace_back(std::make_pair(x,std::cosf(x*this->multiplier)*0.5f));
	}
}

void OglContext::decMultiplier(void)
{
	this->multiplier /= 2.0f;
	this->initGraph();
	this->display();
	PostMessage(this->hWnd, WM_PAINT, 0, 0);
}

void OglContext::incMultiplier(void)
{
	this->multiplier *= 2.0f;
	this->initGraph();
	this->display();
	PostMessage(this->hWnd, WM_PAINT, 0, 0);
}

std::string OglContext::getWidth(void)
{
	return std::to_string(this->width);
}

std::string OglContext::getHeight(void)
{
	return std::to_string(this->height);
}

void OglContext::setOglWinSize(char * width, char * height)
{
	this->width = std::stoi(std::string(width));
	this->height = std::stoi(std::string(height));
	SetWindowPos(this->hWnd, HWND_TOP, 0, 0, this->width, this->height,
		SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE);
	this->initGraph();
	this->display();
}

void OglContext::initControls()
{
	cntrls = new Controls(this->phWnd,this->hInstance);
}

void OglContext::addEditControls(UINT16 xpos, UINT16 ypos, UINT16 width, UINT16 height, UINT16 count)
{
	if (!this->cntrls)
	{
		this->errorMsg("Uninitialized controls");
		return;
	}
	for (UINT16 i = 0; i < count; ++i)
	{
		this->cntrls->addControls(L"EDIT", nullptr, xpos, this->height + 20 + ypos*i,
			width, height, WS_VISIBLE | WS_BORDER | ES_LEFT, 
			std::string("edit_control" + std::to_string(i)));
		this->cntrls->addControls(L"BUTTON", L"Draw", xpos * 2 + width, this->height + 20 + ypos*i, 
			50, height, WS_TABSTOP | BS_DEFPUSHBUTTON | WS_VISIBLE, 
			std::string("ec_button" + std::to_string(i)));
	}
	
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
		//OglContext::display();
		SwapBuffers(hdc);
		//BeginPaint(hWnd, &ps);
		//EndPaint(hWnd, &ps);
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