#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>
#include "Keyboard.h"

#define CALLBACK __stdcall
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define D 0x44 
#define Q 0x51 
#define Z 0x5A 
#define S 0x53 

class Window
{
public:
	Window() = default;
	Window(HINSTANCE hInstance, LPCSTR t);
	~Window();
	
	void Show();
	void Getmessage();
	bool windowShouldClose();
	HWND Gethwnd();
	Keyboard kbd;
	float xMouse = 0.0f;
	float yMouse = 0.0f;
	float wMouse = 3.0f;
private:
	
	static LRESULT CALLBACK SetupMsgPtr(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK handleMsgSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK handleMsg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HINSTANCE hInstance;
	HWND hwnd;
	
	bool _windowShouldClose = false;
	
	
};

