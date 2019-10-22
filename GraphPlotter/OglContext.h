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

	void errorMsg(const char *err);
public:
	OglContext(HINSTANCE hInstance);
	~OglContext();

	ATOM RegisterWinClass(WNDPROC WndProc);
	BOOL InitInstance(int nCmdShow, UINT16 width, UINT16 height, HWND phwnd);
};

