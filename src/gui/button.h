#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <functional>

namespace gui
{
    class Button
    {
        std::function<void(void)> m_func;
        SDL_Point m_point;
        SDL_Rect m_rect;
        SDL_Renderer* m_rend;
        const char* m_title;
        TTF_Font* m_font;

    public:

        Button(const char* title, const SDL_Point& point, const std::function<void(void)>& on_click, SDL_Renderer* rend, TTF_Font* font);

        bool check_click(int mx, int my);
        bool handle_button_click(const SDL_MouseButtonEvent& btn);
        SDL_Texture* draw();
        void call() { m_func(); }
        SDL_Rect& rect() { return m_rect; }
    };

};
