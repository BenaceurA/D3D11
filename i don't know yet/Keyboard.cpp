#include "Keyboard.h"



bool Keyboard::isKeyPressed(int keyCode)
{
	return Key[keyCode];		
}



void Keyboard::SetState(int keyCode,bool _state)
{	
	Key[keyCode] = _state;
}

void Keyboard::clearState()
{
	for (int i = 0; i < sizeof(Key); i++) {
		Key[i] = false;
	}
}

Keyboard::Keyboard()
{
}


Keyboard::~Keyboard()
{
}
