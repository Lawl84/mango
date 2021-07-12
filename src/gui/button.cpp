#include "button.h"

gui::Button::Button(const char* title, const SDL_Point& point, const std::function<void(void)>& on_click, SDL_Renderer* rend, TTF_Font* font, SDL_Color rect_color, SDL_Color text_color)
    : m_func(on_click), m_point(point), m_rend(rend), m_title(title), m_font(font), m_color(rect_color), text_color(text_color)
{
    int tx, ty;
    TTF_SizeText(m_font, title, &tx, &ty);

    m_rect = {
        m_point.x,
        m_point.y,
        tx,
        ty
    };

    SDL_Surface* surface = TTF_RenderText_Blended(m_font, m_title, text_color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_rend, surface);

    if (surface)
    {
        texture = SDL_CreateTextureFromSurface(m_rend, surface);
        SDL_FreeSurface(surface);
    }

    m_tex = texture;
}


void gui::Button::draw()
{
    SDL_RenderFillRect(m_rend, &m_rect);
    SDL_RenderCopy(m_rend, m_tex, nullptr, &m_rect);
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

//gui::Button::~Button()
//{
//    SDL_DestroyTexture(m_tex);
//}