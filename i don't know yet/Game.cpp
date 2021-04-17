#include "Game.h"




Game::Game(HINSTANCE HINST)
	:
	wnd(HINST,"hello")
{
	initGraphics();
}

void Game::initGraphics()
{
	gfx.InitD3D(wnd.Gethwnd());
	gfx.initPipeline();
	gfx.initGraphics();
}

void Game::UpdateFrame()
{

		if (wnd.kbd.isKeyPressed(VK_LEFT)) {
					
			yoffset = yoffset - speed;
			
		}
		if (wnd.kbd.isKeyPressed(VK_RIGHT)) {
						
			yoffset = yoffset + speed;

		}
		if (wnd.kbd.isKeyPressed(VK_UP)) {

			moveforward = true;			
		}
		else
		{
			moveforward = false;
		}
		if (wnd.kbd.isKeyPressed(VK_DOWN)) {
			
			movebackward = true;
		}	
		else
		{
			movebackward = false;
		}
		if (wnd.kbd.isKeyPressed(D))
		{
			light_px = true;
		}
		else
		{
			light_px = false;
		}
		if (wnd.kbd.isKeyPressed(Q))
		{
			light_nx = true;
		}
		else
		{
			light_nx = false;
		}
		if (wnd.kbd.isKeyPressed(Z))
		{
			light_pz = true;
		}
		else
		{
			light_pz = false;
		}
		if (wnd.kbd.isKeyPressed(S))
		{
			light_nz = true;
		}
		else
		{
			light_nz = false;
		}
}

void Game::DrawFrame(double ft)
{
	gfx.clearRenderTarget();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	gfx.renderframe(yoffset, moveforward,movebackward,light_nz,light_pz, light_nx, light_px);
	
	
	ImGui::Begin("FPS",(bool*)1);
	ImGui::Text("%f",ft);
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	gfx.Present();
	wnd.Show();
	
}

bool Game::windowShouldClose()
{
	return wnd.windowShouldClose();
}


void Game::cleanUp()
{
	gfx.CleanD3D();
}

Game::~Game()
{
}
