#pragma once
#include <SDL.h>

namespace gui
{
	class Pen
	{
		SDL_Renderer* m_rend;
		SDL_Color m_color;
	public:
		Pen(SDL_Renderer* rend, SDL_Color color)
			: m_rend(rend), m_color(color) {}

		void draw(SDL_Point p0);
		void set_color(SDL_Color color) { m_color = color; }
		SDL_Color pen_select_color(SDL_Color* queried_color);
		SDL_Color color() { return m_color; }
	};
};