#include "canvas.h"

gui::Canvas::Canvas(const SDL_Rect& rect)
	: m_rect(rect) {}

void gui::Canvas::adjust_to_window(int window_w, int window_h)
{
	m_rect = { (int)((0.1 * window_w) / 2), (int)((0.2 * window_h) / 2), (int)(0.9 * window_w), (int)(0.8 * window_h) };
}