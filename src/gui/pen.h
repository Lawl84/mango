#pragma once
#include <SDL.h>

namespace gui
{
	class Pen
	{
		SDL_Renderer* m_rend;
		SDL_Color m_color;
		int m_thickness = 2;
	public:
		Pen(SDL_Renderer* rend, SDL_Color color)
			: m_rend(rend), m_color(color) {}

		void draw(SDL_Point p0);
		void set_color(SDL_Color color) { m_color = color; }
		void pen_select_color();
		void pen_select_thickness();
		SDL_Color color() { return m_color; }
		int thickness() { return m_thickness; }
	};
};