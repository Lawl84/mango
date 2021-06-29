#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

namespace gui
{
	class TextEntry
	{
		
		SDL_Renderer* m_rend;
		SDL_Rect m_rect;
		SDL_Color m_color;
		TTF_Font* m_font;

	public:
		std::string m_text;
		TextEntry(SDL_Renderer* rend, const SDL_Rect& rect, TTF_Font* font, SDL_Color color = { 200, 200, 200 });
		void draw();
		void render(const char* text);
		SDL_Rect rect() { return m_rect; };
	};

};