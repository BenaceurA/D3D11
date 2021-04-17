#include "Console.h"

bool Console::ConsoleExists = true;

bool Console::OpenConsole()
{
	if (AllocConsole() != 0) { //succeded

		AllocConsole();
		FILE* fdummy;
		freopen_s(&fdummy, "CONIN$", "r", stdin);
		freopen_s(&fdummy, "CONOUT$", "w", stderr);
		freopen_s(&fdummy, "CONOUT$", "w", stdout);

		HANDLE hConIn = CreateFile("CONIN$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		HANDLE hConOut = CreateFile("CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		SetStdHandle(STD_INPUT_HANDLE, hConIn);
		SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
		SetStdHandle(STD_ERROR_HANDLE, hConOut);
		std::wclog.clear();
		std::clog.clear();
		std::wcout.clear();
		std::cout.clear();
		std::wcerr.clear();
		std::cerr.clear();
		std::wcin.clear();
		std::cin.clear();

		ConsoleExists = true;
		return true;
	}
	return false;
}
