#pragma once
#include <SDL.h>

class Mango
{
	SDL_Window* m_window;
	SDL_Renderer* m_rend;
public:
	Mango();
	~Mango();

	void mainloop();
};