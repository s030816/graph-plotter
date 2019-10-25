#pragma once

#ifdef _WIN64
#define GWL_HINSTANCE GWLP_HINSTANCE
#endif

struct ControlsData
{
	UINT16 idName;
	HWND hWnd;
	int id;
};

class Controls
{
private:
	std::vector<ControlsData> cntrls;
	HWND hWnd;
	HINSTANCE hInstance;
public:
	Controls(HWND hWnd, HINSTANCE hInstance);
	virtual ~Controls();

	void addControls(WCHAR *type, WCHAR *text, UINT16 xpos, UINT16 ypos,
		UINT16 width, UINT16 height, UINT32 styles, UINT16 id);
	std::string getEditText(UINT16 id);
	UINT16 getControl(int wmId);
};

