#pragma once
#include <SDL.h>

namespace gui
{

	class Canvas
	{
		SDL_Rect m_rect;
		SDL_Color m_color;
	public:
		Canvas(const SDL_Rect& rect);
		void adjust_to_window(int window_w, int window_h);
		void set_canvas_rect(const SDL_Rect& rect) { m_rect = rect; }
		SDL_Rect rect() { return m_rect; }
	};

};