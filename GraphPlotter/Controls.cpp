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
	UINT16 width, UINT16 height, UINT32 styles, UINT16 id)
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
	cData.id = this->cntrls.size() + 200;
	cData.idName = id;
	this->cntrls.emplace_back(cData);
}

std::string Controls::getEditText(UINT16 id)
{
	char buff[50];
	auto iter = std::find_if(this->cntrls.begin(),
		this->cntrls.end(), [&](const auto& i) {return i.idName == id; });
	if (iter == this->cntrls.end())
	{
		MessageBoxA(nullptr, "No sych control exist", "Error", MB_OK);
	}
	GetWindowTextA(iter->hWnd, buff, 50);
	return std::string(buff);
}

UINT16 Controls::getControl(int wmId)
{
	auto iter = std::find_if(this->cntrls.begin(),
		this->cntrls.end(), [&](const auto& i) {return i.id == wmId; });
	if (iter == this->cntrls.end())
		return 0;
	return iter->idName;
}

