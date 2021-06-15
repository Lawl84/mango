#include <vector>
#include <SDL.h>


namespace utils
{
	bool collides(int mx, int my, SDL_Rect* rect)
	{
		return (mx >= rect->x && mx <= rect->x + rect->w) && (my >= rect->y && my <= rect->y + rect->h);
	}
}