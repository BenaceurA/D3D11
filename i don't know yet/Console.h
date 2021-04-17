#pragma once
#include <Windows.h>
#include <iostream>
class Console
{
public:
	static bool OpenConsole();
private:
	static bool	ConsoleExists;
};

