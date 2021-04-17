#pragma once
#include "Graphics.h"


class Game
{
public:

	Game();
	void UpdateFrame();
	void DrawFrame(double ft);
	bool windowShouldClose();
	void cleanUp();
	~Game();
private:
};

