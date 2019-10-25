#pragma once

#define MAX_LOADSTRING 100

class OglContext
{
private:
	HINSTANCE hInstance;
	HWND hWnd;
	HWND phWnd;
	WCHAR *szTitle = L"test1";
	WCHAR *szWindowClass = L"test2";
	HDC hDC;
	HGLRC hRC;
	std::vector<std::pair<GLfloat, GLfloat> > graph;
	UINT16 width;
	UINT16 height;
	GLfloat multiplier{ 10.0f };
	Controls* cntrls;
	Calculator calc;

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
	std::string getWidth(void);
	std::string getHeight(void);
	void setOglWinSize(char *width, char *height);
	void initControls(void);
	void addEditControls(UINT16 xpos, UINT16 ypos, UINT16 width, UINT16 height, UINT16 count);
	bool parseControlSelections(int wmId);
};

