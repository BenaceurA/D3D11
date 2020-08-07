#pragma once


#include "Graphics.h"
#include "Window.h"

class Game
{
public:

	Game(HINSTANCE HINST);
	void initGraphics();
	void UpdateFrame();
	void DrawFrame();
	bool windowShouldClose();
	void cleanUp();
	~Game();
	Window wnd;
private:
	
	Graphics gfx;
	float speed = 0.05f;
	float xRot = 0.0f;
	float yRot = 0.0f;
	float zRot = 0.0f;
	float xoffset = 0.0f;
	float yoffset = 0.0f;
	float moveforward = false;
	float movebackward = false;
	bool light_px = false;
	bool light_nx = false;
	bool light_pz = false;
	bool light_nz = false;
};

