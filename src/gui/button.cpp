#include "button.h"

gui::Button::Button(const char* title, const SDL_Point& point, const std::function<void(void)>& on_click, SDL_Renderer* rend, TTF_Font* font)
    : m_func(on_click), m_point(point), m_rend(rend), m_title(title), m_font(font)
{
    int tx, ty;
    TTF_SizeText(m_font, title, &tx, &ty);

    m_rect = {
        m_point.x,
        m_point.y,
        tx,
        ty
    };
}


SDL_Texture* gui::Button::draw()
{
    SDL_RenderFillRect(m_rend, &m_rect);
    SDL_Surface* surface = TTF_RenderText_Blended(m_font, m_title, SDL_Color{ 0, 0, 0 });
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_rend, surface);

    if (surface)
    {
        texture = SDL_CreateTextureFromSurface(m_rend, surface);
        SDL_FreeSurface(surface);
    }
    return texture;
}


bool gui::Button::check_click(int mx, int my)
{
    return (mx >= m_rect.x && mx <= m_rect.x + m_rect.w) && (my >= m_rect.y && my <= m_rect.y + m_rect.h);
}


bool gui::Button::handle_button_click(const SDL_MouseButtonEvent& btn)
{
    if (btn.button == SDL_BUTTON_LEFT)
    {
        if (check_click(btn.x, btn.y))
        {
            call();
        }
    }

    return check_click(btn.x, btn.y);
}
