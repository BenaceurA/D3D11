#include "Game.h"

Game::Game()
{

}

void Game::UpdateFrame()
{

}

void Game::DrawFrame(double ft)
{
	Graphics::getInstance()->renderframe();	
	Window::getInstance()->Show();
}

bool Game::windowShouldClose()
{
	return Window::getInstance()->windowShouldClose();
}

void Game::cleanUp()
{
}

Game::~Game()
{
}
