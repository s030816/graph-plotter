#pragma once

#ifdef _WIN64
#define GWL_HINSTANCE GWLP_HINSTANCE
#endif

struct ControlsData
{
	std::string name;
	HWND hWnd;
	HMENU id;
};

class Controls
{
private:
	std::vector<ControlsData> cntrls;
	HWND hWnd;
	HINSTANCE hInstance;
public:
	Controls();
	virtual ~Controls();

	void addControls(WCHAR *type, WCHAR *text, UINT16 xpos, UINT16 ypos,
		UINT16 width, UINT16 height, UINT32 styles, std::string& name);
	void setParent(HWND hWnd, HINSTANCE hInstance);
};

