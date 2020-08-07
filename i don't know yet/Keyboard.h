#pragma once

class Keyboard
{
public:
	class Event {
		enum Type
		{
			Press,

		};
	};
	
	
		
	bool Key[0xFE];
	
public:

	Keyboard(Keyboard&) = delete;
	Keyboard& operator=(Keyboard&) = delete;
	bool isKeyPressed(int keyCode);
	
	void SetState(int keyCode, bool _state);
	void clearState();
	Keyboard();
	~Keyboard();
	
};

