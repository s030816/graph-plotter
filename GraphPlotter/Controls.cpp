#include "stdafx.h"
#include "Controls.h"

Controls::Controls(HWND hWnd, HINSTANCE hInstance)
{
	this->hWnd = hWnd;
	this->hInstance = hInstance;
}

Controls::~Controls()
{
}

void Controls::addControls(WCHAR *type, WCHAR *text, UINT16 xpos, UINT16 ypos, 
	UINT16 width, UINT16 height, UINT32 styles, std::string& name)
{
	HWND hwndc = CreateWindow(
		type,  // Predefined class; Unicode assumed 
		text,      //  text 
		WS_CHILD | styles,  // Styles 
		xpos,         // x position 
		ypos,         // y position 
		width,        // c width
		height,        // c height
		this->hWnd,     // Parent window
		(HMENU)(this->cntrls.size() + 200),
		this->hInstance,
		NULL);  
	if (!hwndc)
	{
		MessageBoxA(nullptr, "Cannot create control", "Error", MB_OK);
		return;
	}
	ControlsData cData;
	cData.hWnd = hwndc;
	cData.id = (HMENU)(this->cntrls.size() + 200);
	cData.name = name;
	this->cntrls.emplace_back(cData);
}

