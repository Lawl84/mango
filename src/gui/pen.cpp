#include "pen.h"

void gui::Pen::draw(SDL_Point p0)
{
	SDL_RenderDrawPoint(m_rend, p0.x, p0.y);
}

//SDL_Color gui::Pen::pen_select_color(SDL_Color* queried_color)
//{
//	bool running = true;
//	SDL_Window* sm_window = SDL_CreateWindow("Select Color", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 300, 300, SDL_WINDOW_SHOWN);
//	SDL_Renderer* sm_rend = SDL_CreateRenderer(sm_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
//
//	SDL_Event evt;
//
//	while (running)
//	{
//		while (SDL_PollEvent(&evt))
//		{
//			switch (evt.type)
//			{
//			case SDL_QUIT:
//				running = false;
//			}
//		}
//	}
//}