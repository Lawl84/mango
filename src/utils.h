#pragma once
#include <SDL.h>
#include <iomanip>
#include <sstream>
#include <string>

namespace utils
{
	bool collides(int mx, int my, const SDL_Rect& rect)
	{
		return (mx >= rect.x && mx <= rect.x + rect.w) && (my >= rect.y && my <= rect.y + rect.h);
	}

	uint32_t hex(SDL_Color color)
	{
		return ((color.r & 0xff) << 16) + ((color.g & 0xff) << 8) + (color.b & 0xff);
	}
}