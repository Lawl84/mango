#pragma once
#include <SDL.h>

class Mango
{
	SDL_Window* m_window;
	SDL_Renderer* m_rend;
	uint32_t* texbuf;
public:
	Mango();
	~Mango();

	void mainloop();
};