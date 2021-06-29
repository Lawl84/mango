#include "textentry.h"
#include "../utils.h"

gui::TextEntry::TextEntry(SDL_Renderer* rend, const SDL_Rect& rect, TTF_Font* font, SDL_Color color)
	:m_rend(rend), m_rect(rect), m_font(font), m_color(color), m_text("") {}

void gui::TextEntry::draw()
{
	SDL_SetRenderDrawColor(m_rend, m_color.r, m_color.g, m_color.b, m_color.a);
	SDL_RenderFillRect(m_rend, &m_rect);
}

void gui::TextEntry::render(const char* text)
{
	int tx, ty;
	TTF_SizeText(m_font, text, &tx, &ty);
	SDL_Rect r = { m_rect.x, m_rect.y, tx, ty };
	utils::render_text(m_rend, r, text, m_font);
	m_text = text;
}