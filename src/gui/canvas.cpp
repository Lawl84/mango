#include "canvas.h"

gui::Canvas::Canvas(const SDL_Rect& rect, SDL_Color color)
	: m_rect(rect), m_color(color) {}

void gui::Canvas::center(int window_w, int window_h)
{
	m_rect = { (window_w - m_rect.w) / 2, (window_h - m_rect.h) / 2, m_rect.w, m_rect.h };
}