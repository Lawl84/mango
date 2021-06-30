#pragma once
#include <SDL.h>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include "gui/button.h"
#include <map>
#include <SDL_ttf.h>
#include <iostream>

namespace utils
{
	inline bool collides(int mx, int my, const SDL_Rect& rect)
	{
		return (mx >= rect.x && mx <= rect.x + rect.w) && (my >= rect.y && my <= rect.y + rect.h);
	}

	inline uint32_t hex(SDL_Color color)
	{
		return ((color.r & 0xff) << 16) + ((color.g & 0xff) << 8) + (color.b & 0xff);
	}

	inline void draw_buttons(std::vector<gui::Button>& buttons)
	{
		for (gui::Button& button : buttons)
		{
			button.draw();
		}
	}

	inline void render_text(SDL_Renderer* rend, const SDL_Rect& rect, const char* text, TTF_Font* font)
	{
		SDL_Surface* surface = TTF_RenderText_Blended(font, text, SDL_Color{ 0, 0, 0 });

        SDL_Texture* texture{ 0 };

        if (surface)
    		texture = SDL_CreateTextureFromSurface(rend, surface);

        if (texture)
    		SDL_RenderCopy(rend, texture, nullptr, &rect);

        if (surface)
            SDL_FreeSurface(surface);

        if (texture)
            SDL_DestroyTexture(texture);
	}

	inline void color_adjust(int& r, int& g, int& b)
	{
		if (r > 255) r = 255;
		if (r < 0) r = 0;

		if (g > 255) g = 255;
		if (g < 0) g = 0;

		if (b > 255) b = 255;
		if (b < 0) b = 0;

	}

	inline void label(SDL_Renderer* rend, const char* text, TTF_Font* font, const SDL_Point& point)
	{
		int x, y;
		TTF_SizeText(font, text, &x, &y);

		SDL_Surface* surface = TTF_RenderText_Blended(font, text, SDL_Color{ 255, 255, 255 });
		SDL_Texture* texture{ 0 };
		SDL_Rect rect = {
			point.x,
			point.y,
			x,
			y
		};

		if (surface)
			texture = SDL_CreateTextureFromSurface(rend, surface);

		if (texture)
			SDL_RenderCopy(rend, texture, nullptr, &rect);

		if (surface)
			SDL_FreeSurface(surface);

		if (texture)
			SDL_DestroyTexture(texture);

	}

}
