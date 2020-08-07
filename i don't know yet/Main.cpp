#pragma once
#include "Game.h"
#include <iostream>
#include <exception>
#include <comdef.h>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	
	AllocConsole();
	FILE *fDummy;
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
	try {
		Game game(hInstance);
		game.initGraphics();

		int FPS = 60;
		float frameDelay = 1000.0f / FPS;
		float framestart;
		float frametime;
		while (!game.windowShouldClose())
		{
			framestart = (float)GetTickCount();

			game.UpdateFrame();
			game.DrawFrame();

			frametime = GetTickCount() - framestart;
			
			if (frameDelay > frametime) {
				Sleep((DWORD)(frameDelay - frametime));
			}
		}
		game.cleanUp();
	}
	catch (std::exception &e) {
		std::cerr << e.what();
		system("PAUSE");
	}
	catch (HRESULT hr) {
		_com_error err(hr);
		LPCTSTR errMsg = err.ErrorMessage();
		std::cerr << errMsg;
		system("PAUSE");
	}
	catch (...) {		
		std::cerr << "tnaket";
	}
	return 0;
	
	
}
