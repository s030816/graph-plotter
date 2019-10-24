#pragma once

#define MAX_LOADSTRING 100

class OglContext
{
private:
	HINSTANCE hInstance;
	HWND hWnd;
	WCHAR *szTitle = L"test1";
	WCHAR *szWindowClass = L"test2";
	HDC hDC;
	HGLRC hRC;
	std::vector<std::pair<GLfloat, GLfloat> > graph;
	UINT16 width;
	UINT16 height;
	GLfloat multiplier{10.0f};

	void errorMsg(const char *err);
public:
	OglContext(HINSTANCE hInstance);
	~OglContext();

	ATOM RegisterWinClass(WNDPROC WndProc);
	BOOL InitInstance(int nCmdShow, UINT16 width, UINT16 height, HWND phwnd);
	void display(void);
	void initGraph(void);
	void decMultiplier(void);
	void incMultiplier(void);
};

