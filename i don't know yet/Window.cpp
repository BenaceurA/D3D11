#include "Window.h"



LRESULT CALLBACK Window::SetupMsgPtr(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	if (uMsg == WM_NCCREATE) {
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&handleMsgSetup));

		return pWnd->handleMsgSetup(hwnd, uMsg, wParam, lParam);
	}
	else { DefWindowProc(hwnd, uMsg, wParam, lParam); }
}

LRESULT Window::handleMsgSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	return pWnd->handleMsg(hwnd, uMsg, wParam, lParam);
}

LRESULT Window::handleMsg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CLOSE:
		_windowShouldClose = true;
		DestroyWindow(hwnd);
		break;
	case WM_KILLFOCUS:
		kbd.clearState();
		break;
	case WM_KEYDOWN:
		if (wParam == VK_LEFT) {
			kbd.SetState(VK_LEFT,true);
		}
		if (wParam == VK_RIGHT) {
			kbd.SetState(VK_RIGHT, true);
		}
		if (wParam == VK_UP) {
			kbd.SetState(VK_UP, true);
		}
		if (wParam == VK_DOWN) {
			kbd.SetState(VK_DOWN, true);
		}
		if (wParam == D /*D*/) {
			kbd.SetState(0x44, true);
		}
		if (wParam == Q /*Q*/) {
			kbd.SetState(0x51, true);
		}
		if (wParam == Z /*Z*/) {
			kbd.SetState(0x5A, true);
		}
		if (wParam == S /*S*/) {
			kbd.SetState(0x53, true);
		}
		break;
	case WM_KEYUP:
		if (wParam == VK_LEFT) {
			kbd.SetState(VK_LEFT,false);
		}
		if (wParam == VK_RIGHT) {
			kbd.SetState(VK_RIGHT, false);
		}
		if (wParam == VK_UP) {
			kbd.SetState(VK_UP, false);
		}
		if (wParam == VK_DOWN) {
			kbd.SetState(VK_DOWN, false);
		}
		if (wParam == D /*D*/) {
			kbd.SetState(0x44, false);
		}
		if (wParam == Q /*Q*/) {
			kbd.SetState(0x51, false);
		}
		if (wParam == Z /*Z*/) {
			kbd.SetState(0x5A, false);
		}
		if (wParam == S /*S*/) {
			kbd.SetState(0x53, false);
		}
		break;
	case WM_MOUSEMOVE:
		if (wParam == MK_MBUTTON)
		{
			xMouse = (float)GET_X_LPARAM(lParam);
			yMouse = (float)GET_Y_LPARAM(lParam);
		}
				
		break;
	case WM_MOUSEWHEEL:
		if (GET_WHEEL_DELTA_WPARAM(wParam)>0)
		{
			wMouse = wMouse + 0.1f;
		}
		else
		{
			wMouse = wMouse - 0.1f;
		}
		break;
	}
	
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Window::Window(HINSTANCE hInstance, LPCSTR t)
	:
	hInstance(hInstance)
{
	
	//Register window class

	const auto CLASS_NAME = "class_name";
	WNDCLASSEX wc = {0};

	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = SetupMsgPtr;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszClassName = CLASS_NAME;
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);

	hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		t,    // Window text
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_WIDTH, SCREEN_HEIGHT,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		this        // Additional application data
	);

}

Window::~Window()
{
}

void Window::Getmessage() {
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

HWND Window::Gethwnd()
{
	return hwnd;
}

void Window::Show() {
	Getmessage();
	ShowWindow(hwnd, SW_SHOW);
}

bool Window::windowShouldClose() {
	return _windowShouldClose;
}


