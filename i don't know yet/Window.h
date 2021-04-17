#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>
#include "Keyboard.h"
#include <memory>
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
	Window();
	void Show();
	void Getmessage();
	bool windowShouldClose();
	void SetHwnd(const HINSTANCE& hInstance);
	void SetName(LPCSTR name);
	HWND Gethwnd();
	Keyboard kbd;
	float xMouse = 0.0f;
	float yMouse = 0.0f;
	float wMouse = 3.0f;

	static std::unique_ptr<Window>& getInstance();
	static void initInstance();

private:
	
	static LRESULT CALLBACK SetupMsgPtr(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK handleMsgSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK handleMsg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HINSTANCE hInstance;
	HWND hwnd;
	LPCSTR name;
	bool _windowShouldClose = false;
	
	static std::unique_ptr<Window> window;
	
};

