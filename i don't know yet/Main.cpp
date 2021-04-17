#pragma once
#include "Game.h"
#include <iostream>
#include <exception>
#include <comdef.h>
#include "Console.h"
#include <chrono>

void initInstances() {
	/* Order is important */
	Window::initInstance(); 
	Graphics::initInstance();
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	initInstances();

	Window::getInstance()->SetHwnd(hInstance);
	Window::getInstance()->SetName("WindowName");

	Console::OpenConsole();
	const double FPS = 60.0;
	double currFps = 0.0;
	double frameDelay = 1000.0 / FPS;
	std::chrono::time_point<std::chrono::steady_clock> Timer;
	std::chrono::duration<double, std::milli> frameTime;

	try {
		Game game;

		Timer = std::chrono::steady_clock::now();

		while (!game.windowShouldClose())
		{	
			if (frameTime.count() >= frameDelay)
			{
				Timer = std::chrono::steady_clock::now();
				currFps = 1000.0 / frameTime.count();

				game.UpdateFrame();
				game.DrawFrame(currFps);

				std::cout << frameTime.count() << std::endl;
				frameTime.zero();
			}

			frameTime = std::chrono::steady_clock::now() - Timer;
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
		std::cerr << "panic";
	}
	return 0;
	
	
}
