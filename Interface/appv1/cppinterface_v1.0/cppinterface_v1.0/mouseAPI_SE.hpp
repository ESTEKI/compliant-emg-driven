#pragma once
#ifndef DATE_H
#define DATE_H
#include <iostream>
#include <stdlib.h>
#include <type_traits> // std::make_signed_t<>
#include <Windows.h>


class MouseAPI {

	int xconsole = 10,
		yconsole = 10,
		dxconsole = 650,
		dyconsole = 650;
	bool isMouseOverConsoleWin;
	HWND consoleWindow;
	POINT pos;
	SHORT keyState;

	static int xDisplacement ;
	static int yDisplacement ;
    
public:
	POINT prevCoor;
	//static bool countClicks;

	/**
	returns mouse displacement in pixels
	*/
	static int MouseAPI::rollAngle;
	static int MouseAPI::pitchAngle;

	static struct MouseData
	{
		long deltaX;
		long deltaY;
		int rollAngle;
		int pitchAngle;
	};
	MouseAPI::MouseData Run();
	void GetDisplacement(long *dx, long *dy);
	POINT GetxyCoord();
	bool MkeyIsDown();
	bool IsMouseOverWin();
	bool HookMouse();
	bool UnHookMouse();
	void InitMouseAPI();
	static int ButtonProcess(int button,bool down);

	
};//end class
#endif
